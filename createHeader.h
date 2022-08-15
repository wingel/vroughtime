
typedef uint32_t craggy_tag_t;
typedef unsigned long size_t;

#define MAKE_TAG(val) (craggy_tag_t) ( \
    (uint32_t)(val)[0] | \
    (uint32_t)(val)[1] << (uint32_t)8 | \
    (uint32_t)(val)[2] << (uint32_t)16 | \
    (uint32_t)(val)[3] << (uint32_t)24 \
    )

#define VRT_TAG_PAD MAKE_TAG("PAD\0")
#define VRT_TAG_VER MAKE_TAG("VER\0")
#define VRT_TAG_SIG MAKE_TAG("SIG\0")
#define VRT_TAG_NONCE MAKE_TAG("NONC")
#define VRT_TAG_MIDP MAKE_TAG("MIDP")
#define VRT_TAG_RADI MAKE_TAG("RADI")
#define VRT_TAG_ROOT MAKE_TAG("ROOT")
#define VRT_TAG_PATH MAKE_TAG("PATH")
#define VRT_TAG_SREP MAKE_TAG("SREP")
#define VRT_TAG_CERT MAKE_TAG("CERT")
#define VRT_TAG_INDX MAKE_TAG("INDX")
#define VRT_TAG_PUBK MAKE_TAG("PUBK")
#define VRT_TAG_MINT MAKE_TAG("MINT")
#define VRT_TAG_MAXT MAKE_TAG("MAXT")
#define VRT_TAG_DELE MAKE_TAG("DELE")

typedef struct CraggyRoughtimeMessage CraggyRoughtimeMessage;
typedef struct CraggyRoughtimeMessageBuilder CraggyRoughtimeMessageBuilder;

/**
 *
 * @param num_tags
 * @return
 */
size_t craggy_messageHeaderLen(size_t num_tags);

/**
 *
 * @param builder
 * @param tag
 * @param data
 * @param len
 * @return
 */
bool craggy_addTagData(CraggyRoughtimeMessageBuilder *builder, craggy_tag_t tag, const uint8_t *data, size_t len);

/**
 *
 * @param numTags
 * @param out
 * @param outLen
 * @param builder
 * @return
 */
bool craggy_createMessageBuilder(size_t numTags, uint8_t *out, size_t outLen, CraggyRoughtimeMessageBuilder **builder);

/**
 *
 * @param builder
 * @param outLen
 * @return
 */
bool craggy_finish(CraggyRoughtimeMessageBuilder *builder, size_t *outLen);

/**
 *
 * @param builder
 */
void craggy_destroyMessageBuilder(CraggyRoughtimeMessageBuilder *builder);
