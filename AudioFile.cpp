#include <stk/FileWvIn.h>
#include <stk/FileRead.h>
#include <stk/FileWrite.h>
#include "AudioFile.hpp"


//#include <stdio.h>
//#include <stdlib.h>

 
void importAudioFile (std::string name_in, std::string name_out)
{
    FILE * infile  = fopen(name_in.c_str(), "rb");
    FILE * outfile = fopen(name_out.c_str(), "wb");
     
    char  buffer[1024];
    
    size_t count_in;

    /* copy from input to output */
    while ((count_in = fread(buffer, 1, sizeof(buffer), infile)))
        fwrite(buffer, 1, count_in, outfile);

    fclose(infile);    
    fclose(outfile);
}


AudioFile::AudioFile (std::string path) : RessourceFile(AUDIO, path)
{
}

AudioFile::~AudioFile ()
{
}

