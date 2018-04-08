<b>УСТАНОВКА SDL2</b><br>
<ul>
<li>cmake --version</li>
<li>скачать исходники SDL2.tar.gz</li>
<li>распоковать в папку game-dev</li>
<li>mkdir SDL2build</li>
<li>cd SDL2build</li>
<li>cmake gui</li>
<li>cmake ../SDL2</li>
</ul>
<b>настройка через cmake-gui</b><br>
<ul>
<li>sudo apt-get install cmake-gui</li>
<li>cmake-gui</li>
<li>в поле Where is the source code: выбираем наш распакованный SDL2</li>
<li>в поле Where to build the binaries: выбираем папку, которую создали SDL2build</li>
<li>нажимем Configure</li>
<li>В открывшемся окне Unix Makefiles, Use default native compiles, Finish</li>
<li>ставим галочку Advanced</li>
<li>ищем CMAKE_BUILD_TYPE и пишем ему значение debug</li>
<li>нажимаем configure, потом Generate</li>
<li>Закрываем cmake-gui</li>
</ul>

<b>Далее установка</b><br>
<ul>
<li>cd game_dev/SDL2build/</li>
<li>make -j4 </li>
<li>sudo make install установить библиотеку
(должно быть прописано: Install configuration "debug")</li>
<li>в линуксе он установить там, куда прописать переменную CMAKE_INSTALL_PREFIX</li>
<li>по умолчанию usr/local/lib/ (usr/local - это где мы сами компилируем и сами собираем)</li>
<li>В папке bin - лежате системные программы</li>
</ul>

sudo updatedb Обновим всю базу данных всех файлов<br>
locate libSDL2.so Найдем libSDL2.so (Линковка будет происходить с той, которая в usr/local/lib)<br>
в HINTS можно указать путь нашей so, чтобы наверняка<br>

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

подключение статической библиотеки<br>
создаем файл main.cxx описан выше<br>
создаем файл CMakeLists.txt<br>


<b>Дополнения:</b><br>
-если нажать CTRL+click на тип объекта, то можно перейти в файл с его реализацией<br>
-sudo make uninstall (Удалить все изменения SDL) вызываем из папки build<br>
-gdb Отладчик<br>
-таблицы импорта и экспорта:<br>
    -после того как собрали проект, переходим в build -> Debug<br>
    - в терминале набираем ldd <имя сборки><br>
    если не показана SDL2, значит она прилинкована к сборке значит SDL2 статическая<br>
    если бы была SDL2 показана в строках вывода, сборке показаны указатели, значит SDL2 динамическая<br>
- nm <имя сборки> | c++filt<br>
- sdl2-config --static-libs (флаги для платформы, чтобы все слинковалось)<br>
- sdl2-confib -libs (флаги для линковки dynamic)<br>
