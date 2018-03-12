#pragma once

#include <string>
#include <array>

using namespace std;

string default_text =
"To Sherlock Holmes she is always THE woman. I have seldom \
heard him mention her under any other name. In his eyes she eclipses \
and predominates the whole of her sex. It was not that he felt \
any emotion akin to love for Irene Adler. All emotions, and that \
one particularly, were abhorrent to his cold, precise but \
admirably balanced mind. He was, I take it, the most perfect \
reasoning and observing machine that the world has seen, but as a \
lover he would have placed himself in a false position. He never \
spoke of the softer passions, save with a gibe and a sneer. They \
were admirable things for the observer--excellent for drawing the \
veil from men's motives and actions. But for the trained reasoner \
to admit such intrusions into his own delicate and finely \
adjusted temperament was to introduce a distracting factor which \
might throw a doubt upon all his mental results. Grit in a \
sensitive instrument, or a crack in one of his own high-power \
lenses, would not be more disturbing than a strong emotion in a \
nature such as his. And yet there was but one woman to him, and \
that woman was the late Irene Adler, of dubious and questionable \
memory.";

constexpr size_t arrsize = 27;

array<char, arrsize> charlist =
{
    ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z'
};

array<float, arrsize> problist =
{
    0.1835f, 0.0640f, 0.0064f, 0.0259f, 0.0260f, 0.1486f, 0.0078f,
    0.0083f, 0.0061f, 0.0591f, 0.0023f, 0.0001f, 0.0465f, 0.0245f,
    0.0623f, 0.0459f, 0.0256f, 0.0081f, 0.0555f, 0.0697f, 0.0572f,
    0.0506f, 0.0100f, 0.0000f, 0.0031f, 0.0021f, 0.0008f
};