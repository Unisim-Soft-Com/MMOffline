#pragma once
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QSvgRenderer>
#include <QString>

/*
    Utility class for creating QIcon from SVG strings.
    Allows dynamic color changes without needing separate icon files.

    Usage:
        QIcon icon = SvgHelper::iconFromSvg(SvgHelper::SVG_DOCUMENT, "#2A7BE4", QSize(48, 48));
*/

namespace SvgHelper
{
// Creates QIcon from SVG string with specified stroke color
inline QIcon iconFromSvg(const QString& svgTemplate, const QString& strokeColor, const QSize& size = QSize(48, 48))
{
    QString svgContent = svgTemplate;
    svgContent.replace("currentColor", strokeColor);

    QSvgRenderer renderer(svgContent.toUtf8());
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    renderer.render(&painter);

    return QIcon(pixmap);
}

// ==================== SVG Templates ====================

// Document nou - file with plus
const QString SVG_DOCUMENT = R"(
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
            <path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>
            <polyline points="14 2 14 8 20 8"/>
            <line x1="12" y1="18" x2="12" y2="12"/>
            <line x1="9" y1="15" x2="15" y2="15"/>
        </svg>
    )";

// Logare - login arrow
const QString SVG_LOGIN = R"(
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
            <path d="M15 3h4a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2h-4"/>
            <polyline points="10 17 15 12 10 7"/>
            <line x1="15" y1="12" x2="3" y2="12"/>
        </svg>
    )";

// Informatia acumulata - document with lines
const QString SVG_LOGS = R"(
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
            <path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>
            <polyline points="14 2 14 8 20 8"/>
            <line x1="16" y1="13" x2="8" y2="13"/>
            <line x1="16" y1="17" x2="8" y2="17"/>
        </svg>
    )";

// Exit - X in circle
const QString SVG_EXIT = R"(
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
            <circle cx="12" cy="12" r="10"/>
            <line x1="15" y1="9" x2="9" y2="15"/>
            <line x1="9" y1="9" x2="15" y2="15"/>
        </svg>
    )";

// Settings - gear
const QString SVG_SETTINGS = R"(
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
            <circle cx="12" cy="12" r="3"/>
            <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/>
        </svg>
    )";

// ==================== Color Constants ====================

namespace Colors
{
const QString ICON_BLUE = "#2A7BE4";
const QString ICON_RED = "#B3261E";
const QString ICON_WHITE = "#FFFFFF";
const QString TEXT_DARK = "#1F2A44";
const QString TEXT_HEADER = "#0F2D5C";
const QString BG_MAIN = "#F3F6FB";
const QString CARD_BG = "#FFFFFF";
const QString CARD_BORDER = "#DDE5F2";
const QString BTN_SETTINGS_BG = "#2A7BE4";
const QString BTN_SETTINGS_BORDER = "#1c5399";
const QString BTN_EXIT_BG = "#FFEAEC";
const QString BTN_EXIT_BORDER = "#FFC3C8";
}
}
