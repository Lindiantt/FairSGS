<?php
error_reporting(0);
if(!array_key_exists('p',$_GET)) die();
$port=intval($_GET['p']);
if(!($port>0&&$port<=65535)) die();
//检查外部端口是否开放
$socket=fsockopen($_SERVER['REMOTE_ADDR'],$port,$errno,$errstr,10);
if(!$socket)
	die('1');
else
{
	$verify=pack('v',0x5678);
	$operation=pack('C',0);
	$cv=pack('v',0);
	stream_set_timeout($socket,10);
	if(!fwrite($socket,$verify.$cv.$operation)) die('1');
	$r=fread($socket,1);
	if($r===false||strlen($r)!=1) die('1');
	else
	{
		$r=unpack('C',$r);
		if($r[1]!=88&&$r[1]!=99) die('1');
	}
}
require('settings.php');
$version=pack('v',1);
//将IP端口转化为二进制
$port=pack('v',$port);
$addrarray=explode('.',$_SERVER['REMOTE_ADDR']);
$addr='';
for($i=3;$i>=0;$i--)
{
	$addr.=pack('C',$addrarray[$i]);
}
$val=$addr.$port;
$time=time();
//读取文件，剔除重复和超时的服务器
$storage=new SaeStorage($access_key,$secret_key);
$file=$storage->read($domain,'servers');
$newfile='';
if($file!==false)
{
	$len=strlen($file);
	if($len%10==2)
	{
		$dup=false;
		$cut=false;
		for($i=2;$i<$len;$i+=10)
		{
			$timestamp=substr($file,$i+6,4);
			$timestamp2=unpack('L',$timestamp);
			if($timestamp2[1]<$time-3600)
			{
				$cut=true;
				break;
			}
			$addrport=substr($file,$i,6);
			if($addrport===$val)
			{
				$dup=true;
				continue;
			}
			$newfile.=$addrport.$timestamp;
		}
		if($len==202)//如果servers文件已满，那么要处理full文件
		{
			if($cut)
			{
				$storage->delete($domain,'full');
			}
			else if(!$dup)
			{
				$filefull=$storage->read($domain,'full');
				$len=strlen($filefull);
				$newfilefull='';
				if($len%10==2)
				{
					for($i=2;$i<$len;$i+=10)
					{
						$timestamp=substr($filefull,$i+6,4);
						$timestamp2=unpack('L',$timestamp);
						if($timestamp2[1]<$time-3600) break;
						$addrport=substr($filefull,$i,6);
						if($addrport===$val) continue;
						$newfilefull.=$addrport.$timestamp;
					}
				}
				$piece=substr($file,192,10);
				$newfile=substr($file,2,190);
				$newfilefull=$version.$piece.$newfilefull;
				$storage->write($domain,'full',$newfilefull);
			}
		}
	}
}
//保存文件
$time=pack('L',$time);
$newfile=$version.$val.$time.$newfile;
$b=$storage->write($domain,'servers',$newfile);
if($b===false)
	die('a');
else
	echo '0';
?>