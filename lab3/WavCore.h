#pragma once

#include <vector>
#include <sstream>
#include "WavException.h"
#include "WavHeaderException.h"

class WavCore {
public:
    WavCore() {
        header = *new WavHeader();
    }

    ~WavCore() {
        free(&header);
        free(&channels_data);
    }

    void createFromFile(const char *filename)
    throw(IOException, BadFormatException, WavHeaderException, UnsupportedFormatException);

    std::string getInfo();

    void makeWavFile(const char *filename)
    throw(IOException, BadParamException, UnsupportedFormatException);

    void makeMono()
    throw(BadParamException);

    void makeReverb(double delay_seconds, float decay)
    throw(BadParamException);

private:
    void nullHeader();

    void checkHeader(size_t file_size_bytes)
    throw(WavHeaderException);

    template<typename T>
    std::string toString(T x) {
        std::stringstream ss;
        ss << x;
        std::string result;
        ss >> result;
        return result;
    }

    void extractDataInt16(const char *filename)
    throw(IOException, WavHeaderException, UnsupportedFormatException);

    void fillHeader(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan)
    throw(BadParamException, UnsupportedFormatException);

    void prefillHeader();

    struct WavHeader {
        char chunkId[4];
        unsigned long chunkSize;
        char format[4];
        char subchunk1Id[4];
        unsigned long subchunk1Size;
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned long sampleRate;
        unsigned long byteRate;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
        char subchunk2Id[4];
        unsigned long subchunk2Size;
    };
    WavHeader header;
    const short headerSize = 44;
    std::vector<std::vector<short>> channels_data;
};
