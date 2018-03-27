# it-academy
устанавливаем gcc:
  gcc --version (проверяем наличие gcc)
  sudo apt-get install gcc
устанавливаем make:
  make --version 
  sudo apt-get install make
устанавливаем cmake:
  cmake --version
  sudo apt-get install cmake
устанавливаем eclipse IDE for c/c++  Developers
  скачается файл с расширением tar.gz в папку Downloads
  mkdir game_dev (создадим папку game_dev на одном уровне с Downloads, куда положим распакованный eclipse)
  cd Downloads (перешли в папку Downloads где лежит eclipse...tar.gz)
  tar -xvf eclipse...tar.gz -C ../game_dev (распаковываем eclipse в папку game_dev)
  rm -rf eclipse...tar.gz (Удаляем файл tar.gz)
  cd ../game_dev (переходим в папку game_dev)
  ls (проверяем наличие eclipse)
  открываем eclipse из папки
  качаем плагин help->Eclipse Marketplace вводим в строку поиска сppstyle, подтверждаем, Install anyway
  также устанавливаем плагины cppcheck, cmake editor, cmake4eclipse
Важно
  file-> New->C/C++ Project ->C++ Managed BUild
  снимаем галочку с Use default location
  выбираем уже существующий проект(папку с сpp и CMakeLists.txt)
  Называем проект также как папку
  Project type->executable->Empty Project->linux GCC
  нажимаем кнопку next
  В окне Select configurations:
  снимаем галочку с release
  нажимаем кнопку Advanced settings
  В появившемся окне:
  resource: Text file encoding ->Other->UTF-8
  c/c++ build->tool chain editor: Current builder -> CMake Builder (portable)
  Нажимаем Apply и Apply and Close
  далее
  Run->Run COnfigurations->C/C++ Application 
  кнопка Search Project
  выбираем наш main (испоняемый файл) Apply Run
устанавливаем clang-format для редактирования текста
  sudo apt-get install clang-format
  clang-format -style=Chromium -dump-config > .clang-format
  этот файл должен лежать в нашем клонированном репозитории
