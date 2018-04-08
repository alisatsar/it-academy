# it-academy
<b>устанавливаем gcc:</b><br>
<ul>
 <li>gcc --version (проверяем наличие gcc)</li>
 <li>sudo apt-get install gcc<br>т</li>
</ul>  
устанавливаем make:<br>
  make --version <br>
  sudo apt-get install make<br>
устанавливаем cmake:<br>
  cmake --version<br>
  sudo apt-get install cmake<br>
устанавливаем eclipse IDE for c/c++  Developers<br>
  скачается файл с расширением tar.gz в папку Downloads<br>
  mkdir game_dev (создадим папку game_dev на одном уровне с Downloads, куда положим распакованный eclipse)<br>
  cd Downloads (перешли в папку Downloads где лежит eclipse...tar.gz)<br>
  tar -xvf eclipse...tar.gz -C ../game_dev (распаковываем eclipse в папку game_dev)<br>
  rm -rf eclipse...tar.gz (Удаляем файл tar.gz)<br>
  cd ../game_dev (переходим в папку game_dev)<br>
  ls (проверяем наличие eclipse)<br>
  открываем eclipse из папки<br>
  качаем плагин help->Eclipse Marketplace вводим в строку поиска сppstyle, подтверждаем, Install anyway<br>
  также устанавливаем плагины cppcheck, cmake editor, cmake4eclipse<br>
Важно<br>
  file-> New->C/C++ Project ->C++ Managed BUild<br>
  снимаем галочку с Use default location<br>
  выбираем уже существующий проект(папку с сpp и CMakeLists.txt)<br>
  Называем проект также как папку<br>
  Project type->executable->Empty Project->linux GCC<br>
  нажимаем кнопку next<br>
  В окне Select configurations:<br>
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
