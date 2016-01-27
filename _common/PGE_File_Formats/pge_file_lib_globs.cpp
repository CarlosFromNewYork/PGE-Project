#include "pge_file_lib_globs.h"
#ifdef _WIN32
#include "windows.h"
#endif
#ifndef PGE_FILES_QT
#include <limits.h> /* PATH_MAX */
#include <sstream>
#include <string>
#include "charsetconvert.h"
#else
#include <QFileInfo>
#endif


namespace PGE_FileFormats_misc
{
#ifndef PGE_FILES_QT

    void split(std::vector<std::string>& dest, const std::string& str, std::string separator)
    {
        #ifdef _MSC_VER
        char* pTempStr = _strdup( str.c_str() );//Microsoft Visual Studio on Windows
        #else
        char* pTempStr = strdup( str.c_str() );//GCC, CLang on Linux and on Mac OS X, or MinGW on Windows
        #endif
        char* pWord = std::strtok(pTempStr, separator.c_str());
        while(pWord != NULL)
        {
            dest.push_back(pWord);
            pWord = std::strtok(NULL, separator.c_str());
        }
        free(pTempStr);
    }

    void replaceAll(std::string& str, const std::string& from, const std::string& to)
    {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    void RemoveSub(std::string& sInput, const std::string& sub)
    {
        std::string::size_type foundpos = sInput.find(sub);
        if ( foundpos != std::string::npos )
            sInput.erase(sInput.begin() + foundpos, sInput.begin() + foundpos + sub.length());
    }

    bool hasEnding (std::string const &fullString, std::string const &ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

    const char HEX2DEC[256] =
    {
        /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
        /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

        /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
    };

    // Only alphanum is safe.
    const char SAFE[256] =
    {
        /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
        /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

        /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
        /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

        /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

        /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };

    PGESTRING url_encode(const std::string & sSrc)
    {
       const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
       const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
       const int SRC_LEN = sSrc.length();
       unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
       unsigned char * pEnd = pStart;
       const unsigned char * const SRC_END = pSrc + SRC_LEN;

       for (; pSrc < SRC_END; ++pSrc)
       {
          if (SAFE[*pSrc])
             *pEnd++ = *pSrc;
          else
          {
             // escape this char
             *pEnd++ = '%';
             *pEnd++ = DEC2HEX[*pSrc >> 4];
             *pEnd++ = DEC2HEX[*pSrc & 0x0F];
          }
       }

       std::string sResult((char *)pStart, (char *)pEnd);
       delete [] pStart;
       return sResult;
    }

    PGESTRING url_decode(const std::string & sSrc)
    {
       // Note from RFC1630: "Sequences which start with a percent
       // sign but are not followed by two hexadecimal characters
       // (0-9, A-F) are reserved for future extension"

       const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
       const int SRC_LEN = sSrc.length();
       const unsigned char * const SRC_END = pSrc + SRC_LEN;
       // last decodable '%'
       const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

       char * const pStart = new char[SRC_LEN];
       char * pEnd = pStart;

       while (pSrc < SRC_LAST_DEC)
       {
          if (*pSrc == '%')
          {
             char dec1, dec2;
             if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
             {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
             }
          }

          *pEnd++ = *pSrc++;
       }

       // the last 2- chars
       while (pSrc < SRC_END)
          *pEnd++ = *pSrc++;

       std::string sResult(pStart, pEnd);
       delete [] pStart;
       return sResult;
    }
#endif


    static const std::string base64_chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

    static inline bool is_base64(unsigned char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    #ifdef PGE_FILES_QT
    std::string  base64_encodeW(const std::wstring &source)
    {
        return base64_encode(reinterpret_cast<const unsigned char*>(source.c_str()), source.size());
    }

    std::wstring base64_decodeW(const std::string &source)
    {
        std::string out=base64_decode(source);
        return std::wstring((wchar_t*)out.c_str());
    }
    #else
    std::string  base64_encodeW(std::string &source)
    {
        SI_ConvertW<wchar_t> utf8(true);
        size_t new_len = utf8.SizeFromStore(source.c_str(), source.length());
        std::wstring t;
        t.resize(new_len);
        if(utf8.ConvertFromStore(source.c_str(), source.length(), (wchar_t*)t.c_str(), new_len))
        {
            return base64_encode(reinterpret_cast<const unsigned char*>(t.c_str()), t.size());
        }
        return "<fail to convert charset>";
    }

    std::string base64_decodeW(std::string &source)
    {
        std::string out=base64_decode(source);
        for(size_t i=0; i<out.size(); i++)
             printf("%i ", (int)out[i]);
        printf("%s", out.c_str());

        std::wstring outw((wchar_t*)out.c_str());
        SI_ConvertW<wchar_t> utf8(true);
        size_t new_len = outw.length()*2;//utf8.SizeToStore(outw.c_str());
        std::string out2;
        out2.resize(new_len);
        if(utf8.ConvertToStore(outw.c_str(), (char*)out2.c_str(), new_len))
        {
            return out2;
        }
        return "<fail to convert charset>";
    }
    #endif

    std::string base64_encode(std::string &source)
    {
        return base64_encode(reinterpret_cast<const unsigned char*>(source.c_str()), source.size());
    }

    std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
    {
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--)
        {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3)
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while((i++ < 3))
                ret += '=';
        }
        return ret;
    }

    std::string base64_decode(std::string const& encoded_string)
    {
        int in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
        {
            char_array_4[i++] = encoded_string[in_]; in_++;
            if (i ==4)
            {
                for (i = 0; i <4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }
        return ret;
    }

    bool TextFileInput::exists(PGESTRING filePath)
    {
        #ifdef PGE_FILES_QT
        return QFile::exists(filePath);
        #else
        FILE *x=fopen(filePath.c_str(), "rb");
        if(x)
        {
            fclose(x); return true;
        }
        return false;
        #endif
    }


    /*****************BASE TEXT I/O CLASS***************************/
    TextInput::TextInput() : _lineNumber(0) {}
    TextInput::~TextInput(){}
    PGESTRING TextInput::read(long){ return ""; }
    PGESTRING TextInput::readLine() { return "";}
    PGESTRING TextInput::readCVSLine() { return ""; }
    PGESTRING TextInput::readAll() { return ""; }
    bool TextInput::eof() { return true; }
    long long TextInput::tell() { return 0; }
    void TextInput::seek(long long, TextInput::positions) {}
    PGESTRING TextInput::getFilePath() { return _filePath; }
    void TextInput::setFilePath(PGESTRING path) { _filePath=path; }
    long TextInput::getCurrentLineNumber() { return _lineNumber; }
    /*****************BASE TEXT I/O CLASS***************************/


    /*****************RAW TEXT I/O CLASS***************************/
    RawTextInput::RawTextInput() : TextInput(), _pos(0), _data(0), _isEOF(true) {}

    RawTextInput::RawTextInput(PGESTRING *rawString, PGESTRING filepath) : TextInput(), _pos(0), _data(0), _isEOF(true)
    {
        open(rawString, filepath);
    }

    RawTextInput::~RawTextInput() {}

    bool RawTextInput::open(PGESTRING *rawString, PGESTRING filepath)
    {
        if(!rawString) return false;
        _data = rawString;
        _filePath = filepath;
        _pos = 0;
        _isEOF = _data->size()==0;
        _lineNumber = 0;
        return true;
    }

    void RawTextInput::close()
    {
        _isEOF=true;
        _data=NULL;
        _filePath.clear();
        _pos=0;
        _lineNumber = 0;
    }

    PGESTRING RawTextInput::read(long len)
    {
        if(!_data) return "";
        if(_isEOF) return "";
        if( (_pos+len) >=(signed)_data->size())
        {
            len = _data->size()-_pos;
            _isEOF=true;
        }
        PGESTRING buf(len + 1, '\0');
        #ifdef PGE_FILES_QT
        buf = _data->mid(_pos, len);
        #else
        buf = _data->substr(_pos, len);
        #endif
        _pos += len;
        return buf;
    }

    PGESTRING RawTextInput::readLine()
    {
        if(!_data) return "";
        if(_isEOF) return "";
        PGESTRING buffer;
        PGEChar cur;
        do{
            cur = (*_data)[(int)_pos++];
            if(_pos>=(signed)_data->size())
                { _pos = _data->size(); _isEOF=true; }
            if((cur!='\r') && (cur!='\n')) buffer.push_back(cur);
        } while((cur != '\n') && (cur!=',') && !_isEOF);
        _lineNumber++;
        return buffer;
    }

    PGESTRING RawTextInput::readCVSLine()
    {
        if(!_data) return "";
        if(_isEOF) return "";
        bool quoteIsOpen=false;
        PGESTRING buffer;
        PGEChar cur;
        do{
            cur = (*_data)[(int)_pos++];
            if(_pos>=(signed)_data->size())
                { _pos = _data->size(); _isEOF=true; }
            if(cur == '\"')
                quoteIsOpen = !quoteIsOpen;
            else
            {
                if((cur != '\r') && (((cur!='\n')&&(cur!=',')) || ( quoteIsOpen )) )
                    buffer.push_back(cur);
                if(cur == '\n') _lineNumber++;
            }
        } while( (((cur != '\n')&&(cur!=','))||quoteIsOpen ) && (!_isEOF) );
        return buffer;
    }

    PGESTRING RawTextInput::readAll()
    {
        if(!_data) return "";
        return *_data;
    }

    bool RawTextInput::eof()
    {
        return _isEOF;
    }

    long long RawTextInput::tell()
    {
        return _pos;
    }

    void RawTextInput::seek(long long pos, TextInput::positions relativeTo)
    {
        if(!_data) return;
        switch(relativeTo)
        {
        case current:
            _pos += pos;
        case end:
            _pos = _data->size()+pos;
        case begin:
        default:
            _pos = pos;
        }
        if(_pos<0) _pos = 0;
        if(_pos>=(signed)_data->size())
            { _pos = _data->size(); _isEOF=true; }
        else
            _isEOF=false;
    }
    /*****************RAW TEXT I/O CLASS***************************/



    /*****************FILE TEXT I/O CLASS***************************/

    TextFileInput::TextFileInput() : TextInput()
    {}

    TextFileInput::TextFileInput(PGESTRING filePath, bool utf8)
    {
        open(filePath, utf8);
    }

    TextFileInput::~TextFileInput()
    {
        #ifdef PGE_FILES_QT
        file.close();
        #else
        if(stream)
        {
            stream.close();
            stream.clear();
        }
        #endif
    }

    bool TextFileInput::open(PGESTRING filePath, bool utf8)
    {
        _filePath = filePath;
        _lineNumber = 0;
        #ifdef PGE_FILES_QT
        bool state=false;
        file.setFileName(filePath);
        state=file.open(QIODevice::ReadOnly|QIODevice::Text);
        if(!state) return false;
        stream.setDevice(&file);
        if(utf8) {
            stream.setCodec("UTF-8");
        } else {
            stream.setAutoDetectUnicode(true);
            stream.setLocale(QLocale::system());
            stream.setCodec(QTextCodec::codecForLocale());
        }
        return true;
        #else
        (void)utf8;
        stream.open(filePath, std::ios::in);
        return (bool)stream;
        #endif
    }

    void TextFileInput::close()
    {
        _filePath.clear();
        _lineNumber = 0;
        #ifdef PGE_FILES_QT
        file.close();
        #else
        stream.close();
        stream.clear();
        #endif
    }

    PGESTRING TextFileInput::read(long len)
    {
        #ifdef PGE_FILES_QT
        if(!file.isOpen()) return "";
        return stream.read(len);
        #else
        if(!stream) return "";
        std::string buf(len + 1, '\0');
        stream.read(&buf[0], len);
        return buf;
        #endif
    }

    PGESTRING TextFileInput::readLine()
    {
        #ifdef PGE_FILES_QT
        if(!file.isOpen()) return "";
        return stream.readLine();
        #else
        if(!stream) return "";
        std::string out;
        std::getline(stream, out);
        if(out.size()==0) return "";
        if(out[out.size()-1]=='\r')
            out.erase(out.size()-1, 1);
        _lineNumber++;
        return out;
        #endif
    }

    PGESTRING TextFileInput::readCVSLine()
    {
        bool quoteIsOpen=false;
        PGESTRING buffer;
        #ifdef PGE_FILES_QT
        QString cur;
        if(!file.isOpen()) return "";
        do{
            cur = stream.read(1);
            if(cur=="\"")
                quoteIsOpen=!quoteIsOpen;
            else
            {
                if((cur!="\r") && (((cur!="\n")&&(cur!=",")) ||(quoteIsOpen)) )
                    buffer.push_back(cur);
                if(cur == "\n") _lineNumber++;
            }
        } while( (((cur != "\n")&&(cur!=","))||quoteIsOpen ) && !stream.atEnd() );
        return buffer;
        #else
        if(!stream) return "";
        char cur;
        do{
            cur = stream.get();
            if(cur=='\"')
                quoteIsOpen = !quoteIsOpen;
            else
            {
                if((cur != '\r') && (((cur!='\n')&&(cur!=',')) || ( quoteIsOpen )) )
                    buffer.push_back(cur);
                if(cur == '\n') _lineNumber++;
            }
        } while( ( ((cur != '\n')&&(cur != ','))||quoteIsOpen ) && (!stream.eof()) );
        return buffer;
        #endif
    }

    PGESTRING TextFileInput::readAll()
    {
        #ifdef PGE_FILES_QT
        return stream.readAll();
        #else
        if(!stream) return "";
        std::string out;
        stream.seekg(0, std::ios::beg);
        char x;
        while(!stream.eof())
        {
            x=stream.get();
            if(x==-1) break;
            if(x!='\r') out.push_back(x);
        }
        return out;
        #endif
    }

    bool TextFileInput::eof()
    {
        #ifdef PGE_FILES_QT
        return stream.atEnd();
        #else
        return stream.eof();
        #endif
    }

    long long TextFileInput::tell()
    {
        #ifdef PGE_FILES_QT
        return stream.pos();
        #else
        return stream.tellg();
        #endif
    }

    void TextFileInput::seek(long long pos, TextFileInput::positions relativeTo)
    {
        #ifdef PGE_FILES_QT
        (void)relativeTo;
        stream.seek(pos);
        #else
        std::ios_base::seekdir s;
        switch(relativeTo)
        {
            case current: s=std::ios_base::cur; break;
            case begin: s=std::ios_base::beg; break;
            case end: s=std::ios_base::end; break;
            default: s=std::ios_base::beg; break;
        }
        stream.seekg(pos, s);
        #endif
    }
    /*****************FILE TEXT I/O CLASS***************************/




    FileInfo::FileInfo()
    {}

    FileInfo::FileInfo(PGESTRING filepath)
    {
        setFile(filepath);
    }

    void FileInfo::setFile(PGESTRING filepath)
    {
        filePath=filepath;
        rebuildData();
    }

    PGESTRING FileInfo::suffix()
    {
        return _suffix;
    }

    PGESTRING FileInfo::filename()
    {
        return _filename;
    }

    PGESTRING FileInfo::fullPath()
    {
        return filePath;
    }

    PGESTRING FileInfo::basename()
    {
        return _basename;
    }

    PGESTRING FileInfo::dirpath()
    {
        return _dirpath;
    }

    void FileInfo::rebuildData()
    {
        #ifdef _MSC_VER
        #define PATH_MAXLEN MAX_PATH
        #else
        #define PATH_MAXLEN PATH_MAX
        #endif
        int i;
        _suffix.clear();
        _filename.clear();
        _dirpath.clear();
        _basename.clear();

        //Take full path
        #ifdef PGE_FILES_QT
        filePath=QFileInfo(filePath).absoluteFilePath();
        #else
        char buf[PATH_MAXLEN + 1];
        char *rez=NULL;
        #ifndef _WIN32
        rez=realpath(filePath.c_str(), buf);
        if(rez) filePath=buf;
        #else
        int ret=0;
        #ifdef UNICODE
        ret=GetFullPathNameA(filePath.c_str(), PATH_MAXLEN, buf, &rez);
        #else
        ret=GetFullPathName(filePath.c_str(), PATH_MAXLEN, buf, &rez);
        #endif
        if(ret!=0) filePath=buf;
        replaceAll(filePath, "\\", "/");
        #endif
        #endif

        //Read directory path
        i=filePath.size()-1;
        for(; i>=0; i--)
        {
            if((filePath[i]=='/')||(filePath[i]=='\\')) break;
        }
        if(i>=0)
        {
           #ifndef PGE_FILES_QT
           _dirpath=filePath.substr(0, i);
           #else
           _dirpath=filePath.left(i);
           #endif
        }

        //Take file suffix
        i=filePath.size()-1;
        for(; i>0; i--)
        {
            if(filePath[i]=='.') break;
        } if(i>0) i++;
        if(i==((signed)filePath.size()-1)) goto skipSuffix;
        for(;i<(signed)filePath.size();i++)
            _suffix.push_back(tolower(PGEGetChar(filePath[i])));
      skipSuffix:

        //Take file name without path
        i=filePath.size()-1;
        for(; i>=0; i--)
        {
            if((filePath[i]=='/')||(filePath[i]=='\\')) break;
        } if(i>0) i++;
        if(i>=((signed)filePath.size()-1)) goto skipFilename;
        for(;i<(signed)filePath.size();i++)
            _filename.push_back(filePath[i]);
      skipFilename:

       //Take base file name
        _basename=_filename;
        for(i=_basename.size()-1; i>0; i--)
        {
            if(_basename[i]=='.')
            {
                _basename.resize(i);
                break;
            }
        }
    }


}

