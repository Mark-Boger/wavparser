#include "gmock/gmock.h"
#include "../../src/audio/WAVParser.h"

#include <fstream>

using namespace testing;

class AWAVParser : public Test
{
  public:
    WAVParser parser;

    // Well formed WAV header
    std::string HeaderChunkID{"\x52\x49\x46\x46", 4};
    std::string HeaderChunkSize{"\x24\x08\x00\x00", 4};
    std::string HeaderFormat{"\x57\x41\x56\x45", 4};

    // Well formed WAV fmt subchunk
    std::string FmtChunkID{"\x66\x6d\x74\x20", 4};
    std::string FmtChunkSize{"\x10\x00\x00\x00", 4};
    std::string FmtFormat{"\x01\x00", 2};
    std::string FmtChannels{"\x02\x00", 2};
    std::string FmtSampleRate{"\x22\x56\x00\x00", 4};
    std::string FmtByteRate{"\x88\x58\x01\00", 4};
    std::string FmtBlockAlign{"\x04\00", 2};
    std::string FmtBits8{"\x08\x00", 2};
    std::string FmtBits16{"\x10\x00", 2};
    std::string FmtBits32{"\x20\x00", 2};

    // Well formed WAV data subchunk
    std::string DataChunkID{"\x64\x61\x74\x61", 4};
    std::string DataChunkSize{"\x00\x08\x00\x00", 4};

    // Malformed WAV header
    std::string BadHeaderID{"\x52\x48\x46\x46", 4};
    std::string BadHeaderFormat{"\x57\x41\x50\x45", 4};

    // Malformed WAV fmt subchunk
    std::string BadFmtID{"\x66\x64\x74\x20", 4};
    std::string BadFmtFormat{"\x02\x00", 2};

    std::string HeaderChunk{HeaderChunkID +
                            HeaderChunkSize +
                            HeaderFormat};

    std::string FmtChunk8{FmtChunkID +
                          FmtChunkSize +
                          FmtFormat +
                          FmtChannels +
                          FmtSampleRate +
                          FmtByteRate +
                          FmtBlockAlign +
                          FmtBits8};

    std::string FmtChunk16{FmtChunkID +
                           FmtChunkSize +
                           FmtFormat +
                           FmtChannels +
                           FmtSampleRate +
                           FmtByteRate +
                           FmtBlockAlign +
                           FmtBits16};

    std::string FmtChunk32{FmtChunkID +
                           FmtChunkSize +
                           FmtFormat +
                           FmtChannels +
                           FmtSampleRate +
                           FmtByteRate +
                           FmtBlockAlign +
                           FmtBits32};

    std::string DataChunk{DataChunkID +
                          DataChunkSize};

    std::istringstream wavFile{HeaderChunk + FmtChunk16 + DataChunk};

    std::istringstream headerWithBadID{BadHeaderID +
                                       HeaderChunkSize +
                                       HeaderFormat};

    std::istringstream headerWithBadFormat{HeaderChunkID +
                                           HeaderChunkSize +
                                           BadHeaderFormat};

    std::istringstream fmtWithBadID{HeaderChunk +
                                    BadFmtID +
                                    FmtChunkSize +
                                    FmtFormat +
                                    FmtChannels +
                                    FmtSampleRate +
                                    FmtByteRate +
                                    FmtBlockAlign +
                                    FmtBits16};

    std::istringstream fmtWithBadFormat{HeaderChunk +
                                        FmtChunkID +
                                        FmtChunkSize +
                                        BadFmtFormat +
                                        FmtChannels +
                                        FmtSampleRate +
                                        FmtByteRate +
                                        FmtBlockAlign +
                                        FmtBits16};

    WAVFile file = parser.parse(wavFile);
    Header header = file.header;
    Fmt fmt = file.fmt;
    Data data = file.data;

    void ASSERT_THROWS_MSG(std::istream &inp, std::string msg)
    {
        try
        {
            parser.parse(inp);
            FAIL() << "Expected InvalidFileException but did not throw error";
        }
        catch (InvalidFileException &e)
        {
            ASSERT_THAT(e.what(), Eq(msg));
        }
        catch (...)
        {
            FAIL() << "Expected InvalidFileException but threw a different error";
        }
    }

    void ASSERT_READS_FILE(std::string fileName)
    {
        std::ifstream file(fileName, std::ios::in | std::ios::binary);

        file.seekg(0, std::ios::end);
        auto total = file.tellg();
        file.seekg(0);

        parser.parse(file);
        auto read = file.tellg();

        ASSERT_THAT(read, Eq(total));
    }

    std::vector<char> randomData(int data_len)
    {
        std::vector<char> dataBuff;

        for (auto i = 0; i < data_len; i++)
            dataBuff.push_back(rand());

        return dataBuff;
    }
};

TEST_F(AWAVParser, ReadsChunkIDCorrectly)
{
    std::string id = WAVFile::toString(header.id);
    ASSERT_THAT(id, Eq("RIFF"));
}

TEST_F(AWAVParser, ReadsChunkSizeCorrectly)
{
    ASSERT_THAT(header.chunkSize, Eq(2084));
}

TEST_F(AWAVParser, ReadsFormatCorrectly)
{
    std::string format = WAVFile::toString(header.format);
    ASSERT_THAT(format, Eq("WAVE"));
}

TEST_F(AWAVParser, ThrowsInvalidFileExceptionWhenGivenIncorectChunkID)
{
    ASSERT_THROW(parser.parse(headerWithBadID), InvalidFileException);
}

TEST_F(AWAVParser, ThrowsInvalidFileExceptionWhenGivenIncorectFormat)
{
    ASSERT_THROW(parser.parse(headerWithBadFormat), InvalidFileException);
}

TEST_F(AWAVParser, ThrowsSpecificErrorMessageWhenGivenIncorrectChunkID)
{
    ASSERT_THROWS_MSG(headerWithBadID, "Invalid ID in header");
}

TEST_F(AWAVParser, ThrowsSpecificErrorMessageWhenGivenIncorrectFormat)
{
    ASSERT_THROWS_MSG(headerWithBadFormat, "Invalid Format in header");
}

TEST_F(AWAVParser, ThrowsInvalidFmtIDMsgWithMalformedFmtID)
{
    ASSERT_THROWS_MSG(fmtWithBadID, "Invalid ID in fmt subchunk");
}

TEST_F(AWAVParser, ThrowsInvalidFmtFormatwithMalformedFmtFormat)
{
    ASSERT_THROWS_MSG(fmtWithBadFormat, "Invalid Format in fmt subchunk");
}

TEST_F(AWAVParser, CanReadBitRawData)
{
    std::vector<char> rData = randomData(2048);
    std::istringstream data16Bit{HeaderChunk +
                                 FmtChunk8 +
                                 DataChunk +
                                 std::string(rData.begin(), rData.end())};

    WAVFile wav = parser.parse(data16Bit);

    ASSERT_THAT(wav.rawData, Eq(rData));
}

TEST_F(AWAVParser, breakItAllCrushYourHopes)
{
    ASSERT_READS_FILE("../tests/data/test_audio.wav");
}

TEST_F(AWAVParser, breakItAllCrushYourHopesAgain)
{
    ASSERT_READS_FILE("../tests/data/test_audio2.wav");
}