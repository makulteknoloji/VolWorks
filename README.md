# SesAyar (VolWorks) 🔊
**Nihayet düzgün çalışan bir ses yöneticisi (en azından benim için)**

[🇬🇧 Click here for English](README.en.md)

Linux için hafif, modern ve kararlı bir sistem tepsisi (system tray) ses yöneticisi. Qt6 ile geliştirilmiş olup tamamen `wpctl` (WirePlumber / PipeWire) için optimize edilmiştir.

## ✨ Özellikler
*   **Overboost Desteği (>%100):** Sesi %100'e sabitleyebilir veya %150'ye kadar aşabilirsiniz. %100'ü geçtiğiniz anda kaydırıcı (slider) otomatik olarak şık bir turuncu/kırmızı renge bürünerek sizi görsel olarak uyarır.
*   **Akıllı Sessize Alma (Orta Tık):** Sisteminizi anında sessize alın. *Akıllı özellik:* Eğer ses %0 iken orta tıklayıp sesi açarsanız, VolWorks sesi otomatik olarak hafifçe yükseltir; böylece %0'da kalıp sağır bir unmute işlemi yaşamazsınız.
*   **Özel Mixer Entegrasyonu:** Sağ tık menüsünü kullanarak favori ses denetleyicinizi anında başlatın (örn: `pavucontrol`, `qpwgraph` veya `xterm -e alsamixer`).
*   **Adaptif Panel İkonları:** Açık renkli bir panel mi kullanıyorsunuz (tint2 veya polybar gibi)? Sadece sağ tıklayın ve **"Koyu İkonlar"** seçeneğini açın. VolWorks dışarıdan resim dosyasına ihtiyaç duymadan ikonları anında yeniden boyar!
*   **Tamamen Özelleştirilebilir:** Ayarlanabilir kaydırma adımları (örn. %2, %5), pencere konumu hafızası ve yerel dil desteği.

## 🚀 Gereksinimler
VolWorks modern Linux ses mimarisini kullandığı için şunlara ihtiyacınız vardır:
*   **WirePlumber** (PipeWire oturum yöneticisi). Uygulama tamamen `wpctl` komutunu kullanır.
*   **Qt5 veya Qt6** (Derlemek için `qt6-base` ve `qt6-tools`)
*   *İsteğe Bağlı:* `pavucontrol` (Gelişmiş ses ayarları penceresi için)

## 🛠️ Kurulum

### Arch Linux (AUR)
*(Yakında AUR'da yerini alacak!)*
```bash
yay -S volworks-git
```

### Kaynaktan Derleme
Dağıtımınızda Qt geliştirici araçlarının yüklü olduğundan emin olun.
```bash
git clone https://github.com/makulteknoloji/volworks.git
cd volworks
qmake6   # (veya dağıtımınıza göre sadece 'qmake')
make
sudo make install
```

## 📸 Ekran Görüntüleri
Eklenecek..

---
**[Makul Teknoloji](https://makulteknoloji.org)** tarafından geliştirilmiştir.
