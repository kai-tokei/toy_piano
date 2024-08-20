#pragma once
#include <Siv3D.hpp> // Siv3D v0.6.15
#include "../structs/note.cpp"

class ToyPiano
{
public:
    ToyPiano()
    {
        // マイクがなかったら、エラー
        if (System::EnumerateMicrophones().isEmpty())
            throw Error{U"No microphone is connected"};

        // 録音されていなかったら、エラー
        if (not mic.isRecording())
            throw Error{U"Failed to start recording"};
    }

    // マイクの拾った音の中で、最も大きい音の音程とオクターブを返す
    Note analize()
    {
        // 最も値の大きい周波数
        float max_freq_size = 0;
        size_t max_freq = 0;
        is_down = false;

        // FFTの結果を取得
        mic.fft(fft);

        // フィルタリング
        for (auto i : step(800))
        {
            const float size = Pow(fft.buffer[i], 0.6f) * 1200;
            if (max_freq < size && size > cut_filter)
            {
                max_freq_size = size;
                max_freq = i;
                is_down = true;
            }
            else if (size > cut_filter)
            {
                is_down = true;
            }
        }

        return convertScaleToString(convertHertzToScale(max_freq * fft.resolution));
    }

    bool isDown()
    {
        return is_down;
    }

private:
    const MicrophoneInfo info = System::EnumerateMicrophones()[0];
    const Microphone mic{5s, Loop::Yes, StartImmediately::Yes};
    FFTResult fft;

    size_t cut_filter = 416;    // カットする周波数の大きさの基準
    bool is_down = false;       // 鍵盤が押されているか

    // ヘルツから音階への変換(110Hzが基準)
    float convertHertzToScale(float hertz)
    {
        if (hertz == 0)
            return 0.0;
        else
            return (12.0 * log(size_t(hertz) / 110.0) / log(2.0));
    }

    // 数値音階を文字列で表す
    Note convertScaleToString(float scale)
    {
        // 十二音階の何倍の精度で音階を見るか
        size_t precision = 2;

        size_t s = int(scale);
        if (scale - s >= 0.5)
            s += 1;
        s *= precision;

        size_t smod = s % (12 * precision); // 音階
        size_t soct = s / (12 * precision); // オクターブ

        const Array<String> value = {
            U"A",
            U"A+",
            U"A#",
            U"A#+",
            U"B",
            U"B+",
            U"C",
            U"C+",
            U"C#",
            U"C#+",
            U"D",
            U"D+",
            U"D#",
            U"D#+",
            U"E",
            U"E+",
            U"F",
            U"F+",
            U"F#",
            U"F#+",
            U"G",
            U"G+",
            U"G#",
            U"G#+",
        };

        return Note{value[smod], soct};
    }
};
