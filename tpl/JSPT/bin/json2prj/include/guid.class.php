<?php
class System  
{  
	function currentTimeMillis()  
	{  
		list($usec, $sec) = explode(" ", microtime());  
		return $sec.substr($usec, 2, 3);  
	}
}

class NetAddress  
{  
	var $Name = 'localhost';  
	var $IP = '127.0.0.1';  
	
	function getLocalHost() // static  
	{
		$address = new NetAddress();  
		$address->Name = $_ENV["COMPUTERNAME"];  
		$address->IP = $_SERVER["SERVER_ADDR"];  
		return $address;  
	} 
	
	function toString()  
	{  
		return strtolower($this->Name.'/'.$this->IP);  
	}  
}
  
class Random  
{  
	function nextLong()
	{  
		$tmp = rand(0,1)?'-':'';  
		return $tmp.rand(1000, 9999).rand(1000, 9999).rand(1000, 9999).rand(100, 999).rand(100, 999);  
	}  
}

// 三段  
// 一段是微秒 一段是地址 一段是随机数  
class Guid
{  
	var $valueBeforeMD5;  
	var $valueAfterMD5;  

	function Guid()  
	{  
		$this->getGuid();  
	}  

	function getGuid()  
	{  
		$address = NetAddress::getLocalHost();  
		$this->valueBeforeMD5 = $address->toString().':'.System::currentTimeMillis().':'.Random::nextLong();  
		$this->valueAfterMD5 = md5($this->valueBeforeMD5);  
	}
	  
	function newGuid()  
	{  
		$Guid = new Guid();  
		return $Guid;  
	}
	  
	function toString()  
	{  
		$raw = strtoupper($this->valueAfterMD5);  
		return substr($raw,0,8).'-'.substr($raw,8,4).'-'.substr($raw,12,4).'-'.substr($raw,16,4).'-'.substr($raw,20);  
	}  
}

function createGuid()
{
	$guid = new Guid();
	return $guid->toString();
}  
?>
