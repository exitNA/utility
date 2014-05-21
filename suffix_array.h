#ifndef SUFFIX_ARRAY_H_
#define SUFFIX_ARRAY_H_

template<typename StrElem = char    // string element type
    , typename SAElem = int>        // suffix array element type
class SuffixArray
{
    class Bucket;
public:
    SuffixArray();
    ~SuffixArray();

    void build(StrElem const* str, size_t length, size_t elem_range);
    void write(char* file_path = NULL);
    void read(char* file_path = NULL);

private:
    void classify(StrElem const* str, size_t length, bool* types);
    void reduce();
    void induceL(StrElem const* str, size_t length, bool* types, Bucket& bkt);
    void induceS();
    bool isLMSStringEqual(StrElem* s1, StrElem* s2);

private:
    SuffixArray(SuffixArray<StrElem, SAElem>& val);
    SuffixArray<StrElem, SAElem>& operator = (SuffixArray<StrElem, SAElem>& val);

private:
    SAElem*     _sa;
    size_t      _length;
};


//////////////////////////////////////////////////////////////////////////
// implement details
//////////////////////////////////////////////////////////////////////////
template<typename StrElem, typename SAElem>
SuffixArray<StrElem, SAElem>::SuffixArray() : _sa(NULL)
, _length(0)
{
    delete[] _sa;
    _sa = NULL;
    _length = 0;
}


template<typename StrElem, typename SAElem>
SuffixArray::~SuffixArray()
{

}


template<typename StrElem, typename SAElem>
void SuffixArray::build(StrElem* str)
{
    // step 1: scan string from right to left to get every element's type
    // step 2: scan string to get LMS-substring
    // step 3: calculate LSM-substring's name to generate new reduced string S1
    // step 4: recursively calculate S1 until every element in S1 is unique
    // step 5: induce SA from S1
}

#endif