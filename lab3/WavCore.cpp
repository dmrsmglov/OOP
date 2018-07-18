#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cmath>
#include "WavCore.h"


using namespace std;

void WavCore::createFromFile(const char *filename)
throw(IOException, BadFormatException, WavHeaderException, UnsupportedFormatException) {
    nullHeader();

    FILE *f = fopen(filename, "rb");
    if (f == nullptr) {
        throw IOException();
    }

    size_t blocks_read = fread(&header, sizeof(WavHeader), 1, f);
    if (blocks_read != 1) {
        throw BadFormatException();
    }

    fseek(f, 0, SEEK_END); // seek to the end of the file
    auto file_size = static_cast<size_t>(ftell(f)); // current position is a file size!
    fclose(f);
    checkHeader(file_size);
    extractDataInt16(filename);
}


std::string WavCore::getInfo() {
    std::string information = "audioFormat  " + toString<unsigned short>(header.audioFormat) + '\n'
                              + "numChannels  " + toString<unsigned short>(header.numChannels) + '\n'
                              + "sampleRate  " + toString<unsigned long>(header.sampleRate) + '\n'
                              + "bitsPerSample " + toString<unsigned short>(header.bitsPerSample) + '\n'
                              + "byteRate " + toString<unsigned long>(header.byteRate) + '\n'
                              + "blockAlign " + toString<unsigned short>(header.blockAlign) + '\n'
                              + "chunkSize " + toString<unsigned long>(header.chunkSize) + '\n'
                              + "subchunk1Size " + toString<unsigned long>(header.subchunk1Size) + '\n'
                              + "subchunk2Size " + toString<unsigned long>(header.subchunk2Size) + '\n';
    return information;
}

void WavCore::extractDataInt16(const char *filename)
throw(IOException, WavHeaderException, UnsupportedFormatException) {
    if (header.bitsPerSample != 16) {
        throw UnsupportedFormatException();
    }

    FILE *f = fopen(filename, "rb");
    if (f == nullptr) {
        throw IOException();
    }
    fseek(f, headerSize, SEEK_SET); // Seek to the begining of PCM data.

    int chan_count = header.numChannels;
    auto samples_per_chan = static_cast<int>((header.subchunk2Size / sizeof(short)) / chan_count);

    std::vector<short> all_channels;
    all_channels.resize(chan_count * samples_per_chan);
    size_t read_bytes = fread(all_channels.data(), 1, header.subchunk2Size, f);
    if (read_bytes != header.subchunk2Size) {
        throw IOException();
    }
    fclose(f);

    channels_data.resize(static_cast<unsigned long>(chan_count));
    for (auto &ch : channels_data) {
        ch.resize(static_cast<unsigned long>(samples_per_chan));
    }

    for (int ch = 0; ch < chan_count; ch++) {
        std::vector<short> &chdata = channels_data[ch];
        for (size_t i = 0; i < samples_per_chan; i++) {
            chdata[i] = all_channels[chan_count * i + ch];
        }
    }
}


void WavCore::checkHeader(size_t file_size_bytes)
throw(WavHeaderException) {
    if (header.chunkId[0] != 'R' ||
        header.chunkId[1] != 'I' ||
        header.chunkId[2] != 'F' ||
        header.chunkId[3] != 'F') {
        throw HeaderRiffException();
    }

    if (header.chunkSize != file_size_bytes - 8) {
        throw HeaderFileSizeException();
    }

    if (header.format[0] != 'W' ||
        header.format[1] != 'A' ||
        header.format[2] != 'V' ||
        header.format[3] != 'E') {
        throw HeaderWaveException();
    }

    if (header.subchunk1Id[0] != 'f' ||
        header.subchunk1Id[1] != 'm' ||
        header.subchunk1Id[2] != 't' ||
        header.subchunk1Id[3] != ' ') {
        throw HeaderFMTException();
    }

    if (header.audioFormat != 1) {
        throw HeaderNotPCMException();
    }

    if (header.subchunk1Size != 16) {
        throw HeaderSubchunk1Exception();
    }

    if (header.byteRate != header.sampleRate * header.numChannels * header.bitsPerSample / 8) {
        throw HeaderBytesRateException();
    }

    if (header.blockAlign != header.numChannels * header.bitsPerSample / 8) {
        throw HeaderBlockAlignException();
    }

    if (header.subchunk2Id[0] != 'd' ||
        header.subchunk2Id[1] != 'a' ||
        header.subchunk2Id[2] != 't' ||
        header.subchunk2Id[3] != 'a') {
        throw HeaderFMTException();
    }

    if (header.subchunk2Size != file_size_bytes - headerSize) {
        throw HeaderSubchunk2SizeException();
    }
}

void WavCore::prefillHeader() {
    header.chunkId[0] = 'R';
    header.chunkId[1] = 'I';
    header.chunkId[2] = 'F';
    header.chunkId[3] = 'F';

    header.format[0] = 'W';
    header.format[1] = 'A';
    header.format[2] = 'V';
    header.format[3] = 'E';

    header.subchunk1Id[0] = 'f';
    header.subchunk1Id[1] = 'm';
    header.subchunk1Id[2] = 't';
    header.subchunk1Id[3] = ' ';

    header.subchunk2Id[0] = 'd';
    header.subchunk2Id[1] = 'a';
    header.subchunk2Id[2] = 't';
    header.subchunk2Id[3] = 'a';

    header.audioFormat = 1;
    header.subchunk1Size = 16;
    header.bitsPerSample = 16;
}

void WavCore::fillHeader(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan)
throw(BadParamException, UnsupportedFormatException) {
    if (bits_per_sample != 16) {
        throw UnsupportedFormatException();
    }

    if (chan_count < 1) {
        throw BadParamException();
    }
    prefillHeader();

    int file_size_bytes = headerSize + chan_count * (bits_per_sample / 8) * samples_count_per_chan;

    header.sampleRate = static_cast<unsigned long>(sample_rate);
    header.numChannels = static_cast<unsigned short>(chan_count);
    header.bitsPerSample = 16;

    header.chunkSize = static_cast<unsigned long>(file_size_bytes - 8);
    header.subchunk2Size = static_cast<unsigned long>(file_size_bytes - headerSize);

    header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = static_cast<unsigned short>(header.numChannels * header.bitsPerSample / 8);
}

void WavCore::makeWavFile(const char *filename)
throw(IOException, BadParamException, UnsupportedFormatException) {
    auto chan_count = (int) channels_data.size();

    if (chan_count < 1) {
        throw BadParamException();
    }

    auto samples_count_per_chan = (int) channels_data[0].size();

    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < chan_count; ch++) {
        if (channels_data[ch].size() != (size_t) samples_count_per_chan) {
            throw BadParamException();
        }
    }

    fillHeader(chan_count, 16, static_cast<int>(header.sampleRate), samples_count_per_chan);
    std::vector<short> all_channels;
    all_channels.resize(chan_count * samples_count_per_chan);

    for (int ch = 0; ch < chan_count; ch++) {
        const std::vector<short> &chdata = channels_data[ch];
        for (size_t i = 0; i < samples_count_per_chan; i++) {
            all_channels[chan_count * i + ch] = chdata[i];
        }
    }

    FILE *f = fopen(filename, "wb");
    fwrite(&header, sizeof(WavHeader), 1, f);
    fwrite(all_channels.data(), sizeof(short), all_channels.size(), f);
    if (f == nullptr) {
        throw IOException();
    }
    fclose(f);
}

void WavCore::nullHeader() {
    memset(&header, 0, sizeof(WavHeader));
}

void WavCore::makeMono()
throw(BadParamException) {
    auto chan_count = (int) channels_data.size();
    if (chan_count != 2) {
        throw BadParamException();
    }
    auto samples_count_per_chan = (int) channels_data[0].size();

    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < chan_count; ch++) {
        if (channels_data[ch].size() != (size_t) samples_count_per_chan) {
            throw BadParamException();
        }
    }

    vector<short> &mono = channels_data[0];
    mono.resize(1);
    mono.resize(static_cast<unsigned long>(samples_count_per_chan));

    for (size_t i = 0; i < samples_count_per_chan; i++) {
        mono[i] = static_cast<short>((channels_data[0][i] + channels_data[1][i]) / 2);
    }
}

void WavCore::makeReverb(double delay_seconds, float decay)
throw(BadParamException) {
    auto chan_count = (int) channels_data.size();

    if (chan_count < 1) {
        throw BadParamException();
    }

    auto samples_count_per_chan = (int) channels_data[0].size();

    // Verify that all channels have the same number of samples.
    for (size_t ch = 0; ch < chan_count; ch++) {
        if (channels_data[ch].size() != (size_t) samples_count_per_chan) {
            throw BadParamException();
        }
    }
    auto delay_samples = (int) (delay_seconds * header.sampleRate);

    for (size_t ch = 0; ch < chan_count; ch++) {
        std::vector<float> tmp;
        tmp.resize(channels_data[ch].size());

        for (size_t i = 0; i < samples_count_per_chan; i++) {
            tmp[i] = channels_data[ch][i];
        }

        // Add a reverb
        for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
            tmp[i + delay_samples] += decay * tmp[i];
        }

        // Find maximum signal's magnitude
        float max_magnitude = 0.0f;
        for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
            if (abs(tmp[i]) > max_magnitude) {
                max_magnitude = abs(tmp[i]);
            }
        }
        // Signed short can keep values from -32768 to +32767,
        // After reverb, usually there are values large 32000.
        // So we must scale all values back to [ -32768 ... 32768 ]
        float norm_coef = 30000.0f / max_magnitude;
        // Scale back and transform floats to shorts.
        for (size_t i = 0; i < samples_count_per_chan; i++) {
            channels_data[ch][i] = (short) (norm_coef * tmp[i]);
        }
    }
}

