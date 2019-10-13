<?php
  if($_GET['view_source']){ highlight_file(__FILE__); exit; }
  if($_GET['ls']){ system("ls -a"); exit; } // to prove for about there is not hidden files in this directory. you dont have to run dirbuster!
  if(isset($_GET['url'])){
    $url = $_GET['url'].".txt";
    if(!filter_var($url, FILTER_VALIDATE_URL)) exit("invalid url");
    if(!preg_match("/http:\/\/pwn.training\//",$url)) exit("invalid server");
    if(preg_match("/@|#|\(|\)|\\$|`|'|\"|_|{|}|\?/",$url)) exit("you are not orange");
    if(parse_url($url)['host'] !== "pwn.training") exit("invalid host");
    if(parse_url($url)['user'] || parse_url($url)['pass'] || parse_url($url)['port']) exit("you are not orange");
    include $url;
  }
  echo "<hr><a href=./?url=http://pwn.training/cat>cat</a> <a href=./?url=http://pwn.training/dog>dog</a> <a href=./?url=http://pwn.training/robots>robots</a> <hr><a href=./?view_source=1>view-source</a> <a href=./?ls=1>ls</a>";
?>
