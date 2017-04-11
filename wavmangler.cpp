#include <fstream>
#include <cstring>
#include <assert.h>


short numbits[] = {
    0x8000,//1 bit
    0xC000,
    0xE000,
    0xF000,
    0xF800,
    0xFC00,
    0xFE00,
    0xFF00,
    0xFF80,
    0xFFC0,
    0xFFE0,
    0xFFF0,
    0xFFF8,
    0xFFFC,
    0xFFFE,
    0xFFFF//16 bit
};


char* filedata;
unsigned int filelength;

unsigned int startofaudio;
unsigned int bytesofaudio;


unsigned int bufferlength;

short channels;
unsigned int samplerate;
short bitspersample;
short bytespersample;


//16-bit samples are stored as
//2's-complement signed integers, ranging from -32768 to 32767
void bitcrush(short crushmask)
{
    assert(bitspersample == 16);
    short* asample = (short*) &filedata[startofaudio];
    unsigned int totnumsamp = bytesofaudio/2;
    for (unsigned int i = 0;i<totnumsamp;i++)
    {
        //short thissamp = asample[i];
        asample[i] = asample[i] & crushmask;
    }

}


bool savewav(const char* filename)
{
    std::fstream outputfile;
    outputfile.open (filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (outputfile.is_open())
    {
        outputfile.seekg (0, std::ios::beg);
        outputfile.write( (const char*)filedata,filelength);
        outputfile.close();
        return true;
    }
    return false;
}




bool loadwav(const char* filename)
{
    if (filename)
    {
        if (filename[0] != '\0')
        {
            std::fstream inputfile;
            inputfile.open (filename, std::ios::in | std::ios::binary|std::ios::ate);//|ios::ate - go to end (to get size with size = file.tellg();)

            if (inputfile.is_open())
            {
                filelength = inputfile.tellg();
                filedata = new char [filelength];
                inputfile.seekg (0, std::ios::beg);
                inputfile.read (filedata, filelength);
                inputfile.close();
            }
            else
            {
                return false;
            }






            assert(filelength > 44);
            assert(!strncmp("RIFF",(const char*)&(filedata[0]),4));
            assert( *((int*)&(filedata[4])) == (filelength-8)  );///zzzz signed? unsigned??
            assert(!strncmp("WAVE",(const char*)&(filedata[8]),4));

            assert(!strncmp("fmt ",(const char*)&(filedata[12]),4));
            assert( *((int*)&(filedata[16])) == 16 );//16 bytes follow for PCM. others not supported yet.
            assert( *((short*)&(filedata[20])) == 1 );

            channels = *((short*)&(filedata[22]));
            samplerate = *((unsigned int*)&(filedata[24]));
            bitspersample = *((short*)&(filedata[34]));
            bytespersample = bitspersample/8;

            /*28        4   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
                32        2   BlockAlign       == NumChannels * BitsPerSample/8
                               The number of bytes for one sample including
                               all channels. I wonder what happens when
                               this number isn't an integer?*/

            //floataudio = false;

            assert(!strncmp("data",(const char*)&(filedata[36]),4));
            bytesofaudio = *((unsigned int*)&(filedata[40]));
            bufferlength = ((bytesofaudio*8)/channels)/bitspersample;//samplesofaudio

            //assert( (bytesofaudio+44) == filelength);// actually 4 x 44??
            startofaudio = 44;

            return true;///loaded ok.


        }

    }
    return false;//no filename
}




int main(int argc, char **argv)
{
    unsigned char crushindex = 0;
    if(argc > 2)
    {

    }
    if(argc > 1)
    {
        if(loadwav(argv[1]))
        {
            bitcrush(numbits[crushindex]);
            savewav("output.wav");
        }
    }
    return 0;
}
