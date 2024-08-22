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

        calPianoHz();
    }

    // マイクの拾ったピアノの音から、ある一定の閾値を超えたNoteを返す
    Array<Note> analize()
    {
        const size_t qSample = 1200;                       // サンプル数
        const size_t perSample = qSample / piano_keys_num; // 1区画ごとのサンプル数
        Array<Note> notes;                                 // 取得したNoteの配列
        bool is_down = false;

        // FFTの結果を取得
        mic.fft(fft);

        // 88区画に分けて各区画で最大の周波数を取得
        for (size_t k = 0; k < piano_keys_num; ++k)
        {
            float max_freq_size = 0; // 各区画の最大周波数の大きさ
            size_t max_freq = 0;     // 各区画の最大周波数のインデックス

            // 各区画の最大の周波数を探す
            for (size_t i = k * perSample; i < (k + 1) * perSample; ++i)
            {
                const float size = Pow(fft.buffer[i], 0.6f) * qSample;
                RectF{Arg::bottomLeft(i, 600), 1, size}.draw(HSV{240 - i});
                if (size > max_freq_size)
                {
                    max_freq_size = size;
                    max_freq = i;
                    is_down = true;
                }
            }
            // 閾値を超えた場合、その区画の最大周波数を Note として記録
            if (max_freq_size > cut_filter)
            {
                Note note = convertScaleToString(convertHertzToScale(max_freq * fft.resolution));
                note.size = max_freq_size;
                notes.push_back(note);
            }
        }
        // 音階ごとにガイド線を引く
        for (auto x : keys)
            Line{x, 0, x, 800}.draw(2);
        Rect{0, 600 - cut_filter, 800, 2}.draw();

        if (notes.size() == 0)
            notes.push_back(Note{U"A", 0});

        return notes;
    }

    bool isDown()
    {
        return is_down;
    }

private:
    const MicrophoneInfo info = System::EnumerateMicrophones()[0]; // マイク情報
    const Microphone mic{5s, Loop::Yes, StartImmediately::Yes};    // マイクの設定
    FFTResult fft;
    Array<float> keys;                // ピアノの半音ごと音階の周波数リスト
    size_t cut_filter = 500;          // カットする周波数の大きさの基準
    bool is_down = false;             // 鍵盤が押されているか
    const size_t piano_keys_num = 88; // ピアノの鍵盤数

    // ヘルツから音階への変換(110Hzが基準)
    float convertHertzToScale(float hertz)
    {
        if (hertz == 0)
            return 0.0;
        else
            return (12.0 * log(size_t(hertz) / 110.0) / log(2.0));
    }

    // ピアノの半音ごとの周波数を計算
    void calPianoHz()
    {
        for (auto i : step(88))
        {
            // 各音階の周波数を計算
            double hz = 27.500 * Pow(Pow(2, 1.0 / 12.0), i);
            // 4分の1音ずらしてから配列に追加
            double adjustedHz = hz; /// Pow(Pow(2, 1.0 / 24.0), 0.5);
            keys.push_back(adjustedHz);
        }
    }

    // 数値音階をNoteで表す
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
