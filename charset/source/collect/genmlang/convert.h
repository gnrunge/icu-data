

#include <vector>
#include <map>
#include <string>

using namespace std;

#ifdef _WIN32

// IMultiLanguage interfaces

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <olestd.h>

#include <mlang.h>

#endif // _WIN32

#include "unicode/utypes.h"

// encoding information from the system

#define CONVERTER_API_IMLANG 1
#define CONVERTER_API_SYSTEM 2

typedef struct _encoding_info {
    char web_charset_name[80];
    char charset_description[80];
} encoding_info;

typedef encoding_info* encoding_info_ptr;

// codepage identifier

#ifdef _WIN32
typedef UINT cp_id;
#endif // _WIN32

// codepage structure information

#define BYTE_INFO_CONTINUE 2
#define BYTE_INFO_END 1

#define MAX_BYTE_LEN 8

typedef struct _byte_info
{
    int8_t byte[0x100];
} byte_info;

typedef byte_info* byte_info_ptr;

// codepage info generated by this program

typedef struct _cp_info 
{
    int max_byte_size;
    int min_byte_size;
    char default_char[8];
    UChar default_uchar;     // What the default_char maps to
    byte_info_ptr byte_info;
} cp_info;

// platform converter wrapper class

#define CONVERTER_USE_SUBST_CHAR 0  // use built-in substitution char
#ifdef _WIN32
#define CONVERTER_USE_DEF_CHAR MLCONVCHARF_USEDEFCHAR // use supplied substitution char
#endif // _WIN32

class converter {
public:
    converter(cp_id cp, encoding_info_ptr enc_info);
    ~converter();
    
    size_t from_unicode(char* target, char* target_limit, const UChar* source, const UChar* source_limit, unsigned int flags = CONVERTER_USE_DEF_CHAR);
    size_t to_unicode(UChar* target, UChar* target_limit, const char* source, const char* source_limit);
    inline int16_t get_status() { return m_err; }
    int get_cp_info(cp_id cp, cp_info& cp_inf);
private:
    string get_default_char(UChar *default_uchar);

    cp_id m_enc_num;
    encoding_info_ptr m_enc_info;
#ifdef _WIN32
    IMultiLanguage2 *m_multilanguage;
    HRESULT m_hr;
    DWORD m_err;
#endif
};

int get_supported_encodings(vector<cp_id>& encodings, map<cp_id, encoding_info_ptr>& encoding_info);

/* The max Unicode value that we collect information on.
   Windows can't support any surrogates yet.
 */
#define MAX_UNICODE_VALUE 0xFFFF


