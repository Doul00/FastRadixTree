#pragma once

#include <vector>
#include <string>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>


#include "../word/word.hh"


class Trie
{
public:
    uint32_t frequency;
    std::string value;
    std::vector<Trie> *children;

    Trie() {}
    Trie(uint32_t frequency, std::string value)
        : frequency(frequency)
        , value(value)
    {
        children = new std::vector<Trie>();
    }

    void add_word_compressed(std::string, uint32_t);
    std::vector<Word> search_close_words(std::string word, int distance);
    std::vector<Word> exact_search(std::string word);

private:
    int write_trie(std::ofstream& stream, Trie& trie);

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
            ar & frequency;
            ar & value;
            ar & children;
    }
};

# include "trie.hxx"

