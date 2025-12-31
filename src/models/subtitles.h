#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <cstdint>
#include <QString>
#include <QList>

namespace Subtitles {

struct SubtitleItem
{
    int64_t start = 0;
    int64_t end = 0;
    QString text;

    bool operator==(const SubtitleItem &other) const {
        return start == other.start && end == other.end && text == other.text;
    }
};

enum SubtitleFormat {
    FORMAT_SRT,
    FORMAT_ASS,
    FORMAT_VTT,
    FORMAT_TTML
};

struct SubtitleTrack
{
    QString name;
    QString language;
    QList<SubtitleItem> items;
    bool enabled = true;
};

} // namespace Subtitles

#endif // SUBTITLES_H
