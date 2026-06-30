# VolWorks 🔊
**Volume manager that actually works (at least for me)**

[🇹🇷 Türkçe açıklama için tıklayın (Read in Turkish)](README.md)

A lightweight, modern, and reliable system tray volume manager for Linux. Built with Qt6 and heavily optimized for `wpctl` (WirePlumber / PipeWire).

## ✨ Features
*   **Overboost Support (>100%):** Limit the volume to standard 100% or allow boosting up to 150%. The slider automatically turns into an elegant orange/red gradient when boosting past 100% to let you know you're in the loud zone.
*   **Smart Mute (Middle Click):** Instantly mute/unmute your system. *Smart feature:* If your volume is at 0% and you middle-click to unmute, VolWorks intelligently bumps the volume up slightly so you actually hear something instead of staying at absolute zero.
*   **Custom Mixer Integration:** Use the right-click tray menu to instantly open your preferred audio mixer (e.g., `pavucontrol`, `qpwgraph`, or even a terminal CLI like `xterm -e alsamixer`).
*   **Adaptive Panel Icons:** Using a light-colored desktop panel (like tint2 or polybar)? Just right-click and enable **"Dark Icons"**. VolWorks will re-paint the icons on the fly without needing additional image files!
*   **Fully Configurable:** Adjustable scroll steps (e.g., 2%, 5%), persistent window geometry memory, and native multi-language support.

## 🚀 Prerequisites
Since VolWorks uses modern Linux audio architecture, you need:
*   **WirePlumber** (PipeWire's session manager). This tool relies completely on the `wpctl` command.
*   **Qt5 or Qt6** (`qt6-base` and `qt6-tools` for compiling)
*   *Optional:* `pavucontrol` (for GUI mixer integration)

## 🛠️ Installation

### Arch Linux (AUR)
*(Will be available on AUR soon!)*
```bash
yay -S volworks-git
```

### Manual Build
Make sure you have Qt development tools installed on your distribution.
```bash
git clone https://github.com/makulteknoloji/volworks.git
cd volworks
qmake6   # (or simply 'qmake' depending on your distro)
make
sudo make install
```

## 📸 Screenshots
TODO

---
Developed by **[Makul Teknoloji](https://makulteknoloji.com)**
