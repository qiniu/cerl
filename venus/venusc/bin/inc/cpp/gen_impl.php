<?php
	/*
		Generate Server hpp
	*/
	//include 'functions.php';
	
	function genSvrHpp($server, $indent)
	{
		global $class_postfix_impl, $class_postfix_base;
		
		$class_name = $server->name;
		$impl_class_name = $server->name . $class_postfix_impl;
		echo "\n${indent}class $impl_class_name : public cerl::Server, public ${class_name}${class_postfix_base} \n";
		echo "${indent}{\n";
/*		echo "${indent}protected:\n";
		$indent2 = $indent . "\t";
			echo "${indent2}cerl::Fiber const self;\n";
*/
		$indent2 = $indent . "\t";
		echo "${indent}public:";
		$sentences = @$server->sentences;
		if ($sentences)
		{
			$user_def_ctor = false;
			foreach ($sentences as $sent)
			{				
				$ctor = @$sent->ctor;
				if ($ctor)
				{
					$user_def_ctor = true;
					break;
				}	
			}
			
			if (!$user_def_ctor)
				genCtor($impl_class_name, NULL, $indent2);
			
			foreach ($sentences as $sent)
			{
				$ctor = @$sent->ctor;
				if ($ctor)
				{
					genCtor($impl_class_name, $ctor, $indent2);
					continue;
				}
				
				genImplFunc(@$sent->function, $indent2);
			}
		}
		
		echo "\n${indent2}void cerl_call _run(cerl::Fiber self, SOCKET socketListen);\n";
		
/*		$ctors_impl = parseCtors($server);
		
		if (count($ctors_impl))
		{
			foreach ($ctors_impl as $ctor)
				genStartDecl($indent2, $ctor);
		}
		else
		{
			genStartDecl($indent2, NULL);
		}
*/
		echo "${indent}};\n";
	}
	
	function genImplFunc($func, $indent)
	{
		if(!$func)
			return;
			
		$args = parseArgs(@$func->args);
		$name = $func->name;
		$async = @$func->async;
		$rettype = getRetType($name);
		
		if ($async)
			echo "\n${indent}/*[async]*/";
			
		echo "\n${indent}void cerl_call $name(";
		
		if (count($args) || !$async)
			echo "\n{$indent}\t";
			
		$fPrefixUnderline = false;
		foreach ($args as $var => $tp)
		{
			$varstr = strtolower($var);
			if ($varstr == "result" || $varstr == "mail")
				$fPrefixUnderline = true;
		}
		
		$var_from = $fPrefixUnderline ? "_mail" : "mail";
		$var_result = $fPrefixUnderline ? "_result" : "result";
		
		echo "Caller* caller";
		if (!$async)
			echo ",\n{$indent}\t$rettype& $var_result";
				
		$args = parseArgs(@$func->args);
		foreach ($args as $var => $tp)
		{
			$typename = mapType($tp, "&");
			echo ",\n${indent}\t";
			echo "const $typename $var";
		}
		echo ")\n";
		echo "${indent}{\n${indent}}\n";
	}
	
/*	function genStartDecl($indent, $ctor)
	{
		echo "\n";
		echo "${indent}static cerl::Fiber cerl_call _start(";
		if (count($ctor))
			echo "\n${indent}\t";
		echo "cerl::Fiber self";
		
		if (count($ctor))
		{
			foreach($ctor as $var => $tp)
			{
				$typename = mapType($tp, "&");
				echo ",\n${indent}\tconst $typename $var";
			}
		}
		echo ");\n";
	}
*/
	function genCtor($class_name, $ctor, $indent)
	{
		echo "\n";
		echo "${indent}";
		if (!count($ctor) || !(@$ctor->args))
			echo "explicit ";
		//echo "${class_name}(cerl::Fiber self";
		echo "${class_name}(";
		if (count($ctor))
		{
			$args = parseArgs(@$ctor->args);
			$i = 0;
			foreach($args as $var => $tp)		
			{
				if ($i == 0)
				{
					$typename = mapType($tp, "&");
					echo "const $typename $var";
				}
				else
				{
					$typename = mapType($tp, "&");
					echo ", const $typename $var";
				}
				$i = $i + 1;
			}
		}
		//echo ")\n${indent}\t: self(_me)\n";
		echo ")\n";
		echo "${indent}{\n${indent}}\n";
	}
?>
<?php
	ob_start();
	
	foreach (@$doc->sentences as $sent)
	{
		$server = @$sent->server;
		if (!$server)
			continue;
			
		$svr_name = $server->name;
		$current_server = $svr_name;
		
		$header = strtoupper("${module}_${svr_name}_impl_h");
		
		echo "/*\n";
		echo "\tDescription: \n";	
		echo "\tAuthor:			SDL Compiler\n";
		/*echo "\tDate:			";
		$datetime = time() + 28800; echo date("Y-M-jS H:i:s\n",$datetime);*/
		echo "*/\n\n";
		echo "#ifndef $header\n";
		echo "#define $header\n";
		$guard = strtoupper("sdl_${module}_base_h");
		echo "\n#ifndef $guard\n";
		echo "#include \"sdl_${module}_base.h\"\n";
		echo "#endif\n";
	
		echo "\n#ifndef VENUS_SERVER_H\n";
		echo "#include <venus/Server.h>\n";
		echo "#endif\n";
	
		echo "\nnamespace $module {\n";
		
		$current_server = $server->name;
		genSvrHpp($server, "");			
		
		echo "\n} //namespace\n";
		echo "\n#include \"${svr_name}Stub.h\"\n";
		echo "\n#endif /* $header */ \n";
		
		$file_name = $svr_name . "Impl.h.in";
		$fd = fopen($file_name, 'w');
		fwrite($fd, ob_get_contents());
		fclose($fd);
		ob_clean();
	}
	
	ob_end_flush();
?>