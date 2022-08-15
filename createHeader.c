
#include <stdbool.h>
#include <stdint.h>

#include "createHeader.h"

struct CraggyRoughtimeMessage {
    const uint8_t *buffer;
    size_t bufferLen;
    uint32_t numTags;
    uint8_t *tags;
    uint8_t *offsets;
    uint8_t *data;
    size_t dataLen;
    bool valid;
};

struct CraggyRoughtimeMessageBuilder {

    const uint8_t *out;
    size_t outLen;

    uint32_t numTags;
    uint32_t tagsAdded;

    uint32_t nextTagDataOffset;

    bool havePreviousTag;
    craggy_tag_t previousTag;

    size_t len;
    size_t headerLen;

    uint8_t *tags;
    uint8_t *offsets;
    uint8_t *data;

    bool valid;
};

size_t craggy_messageHeaderLen(size_t num_tags) {
    return sizeof(uint64_t) + sizeof(uint32_t) /* tag count */ + sizeof(uint32_t) * numMessageOffsets(num_tags) /* offsets */ +
           sizeof(craggy_tag_t) * num_tags /* tag values */;
}

bool craggy_createMessageBuilder(const size_t numTags, uint8_t *out, size_t outLen,
                                 CraggyRoughtimeMessageBuilder **builder) {

    size_t headerLen = craggy_messageHeaderLen(numTags);

    if (outLen < sizeof(uint32_t) || outLen < headerLen || 0xffff < numTags) {
        return NULL;
    }

    *builder = craggy_calloc(1, sizeof(CraggyRoughtimeMessageBuilder));
    (*builder)->valid = false;

    const uint32_t numTags32 = numTags;
    craggy_memcpy(out, &numTags32, sizeof(uint32_t));

    (*builder)->headerLen = headerLen;
    (*builder)->out = out;
    (*builder)->data = out + headerLen;
    (*builder)->len = outLen - headerLen;

    (*builder)->offsets = out + sizeof(uint32_t);
    (*builder)->tags = out + sizeof(uint32_t) * (1 + numMessageOffsets(numTags));

    (*builder)->numTags = numTags;
    (*builder)->out = out;
    (*builder)->outLen = outLen;

    (*builder)->valid = true;

    return true;
}

bool craggy_addTag(CraggyRoughtimeMessageBuilder *builder, uint8_t **out_data, craggy_tag_t tag, size_t len) {

    if (!builder->valid || len % 4 != 0 || builder->len < len || builder->tagsAdded >= builder->numTags ||
        (builder->havePreviousTag && tag <= builder->previousTag)) {
        return false;
    }

    craggy_memcpy(builder->tags + sizeof(uint32_t) * builder->tagsAdded, &tag, sizeof(craggy_tag_t));
    if (builder->tagsAdded > 0) {
        const uint32_t offset_32 = builder->nextTagDataOffset;
        craggy_memcpy(builder->offsets + sizeof(uint32_t) * (builder->tagsAdded - 1), &offset_32, sizeof(uint32_t));
    }

    builder->tagsAdded++;
    builder->previousTag = tag;
    builder->havePreviousTag = true;
    *out_data = builder->data;

    builder->nextTagDataOffset += len;
    builder->len -= len;
    builder->data += len;

    return true;
}

bool craggy_addTagData(CraggyRoughtimeMessageBuilder *builder, craggy_tag_t tag, const uint8_t *data, size_t len) {
    uint8_t *out;
    if (!craggy_addTag(builder, &out, tag, len)) {
        return false;
    }
    craggy_memcpy(out, data, len);
    return true;
}

bool craggy_finish(CraggyRoughtimeMessageBuilder *builder, size_t *outLen) {
    if (!builder->valid || builder->tagsAdded != builder->numTags) {
        return false;
    }
    *outLen = builder->headerLen + builder->nextTagDataOffset;
    builder->valid = false;
    return true;
}

void craggy_destroyMessageBuilder(CraggyRoughtimeMessageBuilder *builder) {
    craggy_free(builder);
}