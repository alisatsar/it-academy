
<a id="github-button" class="btn btn-block btn-social btn-github">
    <i class="fa fa-github"></i> Sign in with GitHub
</a>
# it-academy
<form action="https://travis-ci.org/alisatsar/it-academy/builds">
    <input type="submit" value="BUILD HISTORY" />
</form>
<b>устанавливаем gcc:</b><br>
<ul>
 <li>gcc --version (проверяем наличие gcc)</li>
 <li>sudo apt-get install gcc<br></li>
</ul>  
<b>устанавливаем make:</b><br>
<ul>
  <li>make --version </li>
  <li>sudo apt-get install make</li>
 </ul>  
<b>устанавливаем cmake:</b><br>
<ul>
 <li>cmake --version</li>
 <li>sudo apt-get install cmake</li>
 </ul>  
<b>устанавливаем eclipse IDE for c/c++  Developers</b><br>
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
<b>Важно</b><br>
  file-> New->C/C++ Project ->C++ Managed BUild<br>
  снимаем галочку с Use default location<br>
  выбираем уже существующий проект(папку с сpp и CMakeLists.txt)<br>
  Называем проект также как папку<br>
  Project type->executable->Empty Project->linux GCC<br>
  нажимаем кнопку next<br>
  В окне Select configurations:<br>
  снимаем галочку с release<br>
  нажимаем кнопку Advanced settings<br>
  В появившемся окне:<br>
  resource: Text file encoding ->Other->UTF-8<br>
  c/c++ build->tool chain editor: Current builder -> CMake Builder (portable)<br>
  Нажимаем Apply и Apply and Close<br>
  далее<br>
  Run->Run COnfigurations->C/C++ Application <br>
  кнопка Search Project<br>
  выбираем наш main (испоняемый файл) Apply Run<br>
<b>устанавливаем clang-format для редактирования текста</b><br>
  sudo apt-get install clang-format<br>
  clang-format -style=Chromium -dump-config > .clang-format<br>
  этот файл должен лежать в нашем клонированном репозитории<br><br><br>


 <a href="https://github.com/alisatsar/it-academy/tree/master/test/02-1-sdl-static"><b>ссылка на установку библиотеки SDL2</b></a><br>
 <a href="https://infobsd.ru/sites/default/files/git-cheatsheet-ru.pdf"><b>команды Git</b></a>
