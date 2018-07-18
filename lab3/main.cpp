#include <iostream>
#include <string>
#include "WavException.h"
#include "WavCore.h"

using namespace std;


int main()
{
    const char* input_fname = "0.wav";
    const char* output_fname = "out.wav";
    try {
        WavCore wavCore;

        wavCore.createFromFile(input_fname);
        cout << wavCore.getInfo();
        // #### Make several changes to PCM data.
        // # Making signal mono from stereo.
        wavCore.makeMono();

        // #### Making new WAV file using edited PCM data.

        wavCore.makeWavFile(output_fname);
        // #### Reading the file just created to check its header corectness.
        wavCore.createFromFile(output_fname);
        cout << wavCore.getInfo();
    }
    catch (WavException e) {
        cout << e.what() << endl;
    }
    return 0;
}