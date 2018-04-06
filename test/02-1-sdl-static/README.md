cmake --version
скачать исходники SDL2.tar.gz
распоковать в папку game-dev
mkdir SDL2build
cd SDL2build
cmake gui
cmake ../SDL2

sudo apt-get install cmake-gui
в поле Where is the source code: выбираем наш распакованный SDL2
в поле Where to build the binaries: выбираем папку, которую создали SDL2build
нажимем Configure
В открывшемся окне Unix Makefiles, Use default native compiles, Finish
ставим галочку Advanced
ищем CMAKE_BUILD_TYPE и пишем ему значение debug
нажимаем configure, потом Generate
Закрываем cmake-gui
cd game_dev/SDL2build/
make -j4 
sudo make install установить библиотеку
должно быть прописано: Install configuration "debug"
в линуксе он установить там, куда прописать переменную CMAKE_INSTALL_PREFIX
по умолчанию usr/local/lib/ (usr/local - это где мы сами компилируем и сами собираем)
В папке bin - лежате системные программы

sdl2-config --static-libs (флаги для платформы, чтобы все слинковалось)
sdl2-confib -libs (флаги для линковки dynamic)

sudo updatedb Обновим всю базу данных всех файлов
locate libSDL2.so Найдем libSDL2.so (Линковка будет происходить с той, которая в usr/local/lib)
в HINTS можно указать путь нашей so, чтобы наверняка

другой путь:
удалить ранее установленный SDL
sudo apt search SDL2 найти пакеты с таким названием
sudo apt -remove sdl2-dev
но нужно удалить пакет без зависимостей

для правильной сборки приложения с библиотекой
надо сначала слинковать библиотеку, потом заинстались
потом скомпилировать с приложением

запускаем eclipse
создаем новый проект
после нажатия advanced, устнавливаем еще одну опцию
c/c++ General -> Preprocessor include Paths, Macros etc.
находим СMAKE_EXPORT_COMPILE_COMMANDS Parser Ставим галочку и перемещаем вверх
для того, чтобы сработало первее cmake parser
в CDT GCC Built-in Compiler Settings дописываем -std=c++17
Нажимаем apply и apply and close. нажимаем finish

подключение статической библиотеки
создаем файл main.cxx описан выше
создаем файл CMakeLists.txt


Дополнения:
-если нажать CTRL+click на тип объекта, то можно перейти в файл с его реализацией
-sudo make uninstall (Удалить все изменения SDL) вызываем из папки build
-gdb Отладчик
-таблицы импорта и экспорта:
    -после того как собрали проект, переходим в build -> Debug
    - в терминале набираем ldd <имя сборки>
    если не показана SDL2, значит она прилинкована к сборке значит SDL2 статическая
    если бы была SDL2 показана в строках вывода, сборке показаны указатели, значит SDL2 динамическая
- nm <имя сборки> | c++filt
