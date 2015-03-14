<?php
	/*
		Generate Client Proxy of RPC
	*/	
	
	function genFuncDef($func, $indent)
	{
		$name = $func->name;
		$async = @$func->async;
		$args_set = parseArgs(@$func->args);
		$retType = getRetType($name);

		if ($async)
			echo "\n$indent/*[async]*/";
		
		echo "\n${indent}bool cerl_call $name(";
			
		if (!$async)
		{
			echo "cerl::ScopedAlloc& alloc";
			echo ",\n${indent}\t${retType}& _result";
		}
		
		$flag = 0;
		foreach ($args_set as $var => $type)
		{
			$typename = mapType($type, "&");
			if ($flag == 0 && $async)
			{				
				echo "const ${typename} ${var}";
				$flag = 1;
			}
			else
				echo ",\n${indent}\tconst ${typename} ${var}";
		}
			
		if(count($args_set) || !$async)
			echo "\n${indent}\t";
		echo ")\n";
			
		echo "${indent}{\n";
			
		if (count($args_set) > 1)
		{
			$argsTyName = getArgsType($name);
			echo "${indent}\t$argsTyName _args = {";
			$j = 1;
			foreach ($args_set as $var => $type)
			{
				if ($j == 1)
					echo "$var";
				else
					echo ", $var";
				$j++;
			}
			echo "};\n";
		}
			
		if($async)
		{
			if (count($args_set) == 0)
				echo "${indent}\treturn cerl::Connection::dbg_cast0(this, (cerl::FID)code_${name});\n";
			else if (count($args_set) == 1)
			{
				foreach ($args_set as $var => $type)
					break;
				echo "${indent}\treturn cerl::Connection::dbg_cast(this, (cerl::FID)code_${name}, $var);\n";
			}
			else
				echo "${indent}\treturn cerl::Connection::dbg_cast(this, (cerl::FID)code_${name}, _args);\n";
		}
		else
		{
			if (count($args_set) == 0)
				echo "${indent}\treturn cerl::Connection::dbg_call0(this, alloc, _result, (cerl::FID)code_${name});\n";
			else if (count($args_set) == 1)
			{
				foreach ($args_set as $var => $type)
					break;
				echo "${indent}\treturn cerl::Connection::dbg_call(this, alloc, _result, (cerl::FID)code_${name}, $var);\n";
			}
			else
				echo "${indent}\treturn cerl::Connection::dbg_call(this, alloc, _result, (cerl::FID)code_${name}, _args);\n";
		}	
		echo "${indent}}\n";
	}
	
	function genSyncFuncDef($func, $indent)
	{
		$name = $func->name;
		$async = @$func->async;
		$args_set = parseArgs(@$func->args);
		$retType = getRetType($name);

		if ($async)
			echo "\n$indent/*[async]*/";
		
		echo "\n${indent}bool cerl_call $name(";
			
		if (!$async)
		{
			echo "cerl::ScopedAlloc& alloc";
			echo ",\n${indent}\t${retType}& _result";
		}
		
		$flag = 0;
		foreach ($args_set as $var => $type)
		{
			$typename = mapType($type, "&");
			if ($flag == 0 && $async)
			{				
				echo "const ${typename} ${var}";
				$flag = 1;
			}
			else
				echo ",\n${indent}\tconst ${typename} ${var}";
		}
			
		if(count($args_set) || !$async)
			echo "\n${indent}\t";
		echo ")\n";
			
		echo "${indent}{\n";
			
		if (count($args_set) > 1)
		{
			$argsTyName = getArgsType($name);
			echo "${indent}\t$argsTyName _args = {";
			$j = 1;
			foreach ($args_set as $var => $type)
			{
				if ($j == 1)
					echo "$var";
				else
					echo ", $var";
				$j++;
			}
			echo "};\n";
		}
			
		if($async)
		{
			if (count($args_set) == 0)
				echo "${indent}\treturn m_conn.dbg_cast0(this, (cerl::FID)code_${name});\n";
			else if (count($args_set) == 1)
			{
				foreach ($args_set as $var => $type)
					break;
				echo "${indent}\treturn m_conn.dbg_cast(this, (cerl::FID)code_${name}, $var);\n";
			}
			else
				echo "${indent}\treturn m_conn.dbg_cast(this, (cerl::FID)code_${name}, _args);\n";
		}
		else
		{
			if (count($args_set) == 0)
				echo "${indent}\treturn m_conn.dbg_call0(this, alloc, _result, (cerl::FID)code_${name});\n";
			else if (count($args_set) == 1)
			{
				foreach ($args_set as $var => $type)
					break;
				echo "${indent}\treturn m_conn.dbg_call(this, alloc, _result, (cerl::FID)code_${name}, $var);\n";
			}
			else
				echo "${indent}\treturn m_conn.dbg_call(this, alloc, _result, (cerl::FID)code_${name}, _args);\n";
		}	
		echo "${indent}}\n";
	}
	
	function genProxyClass($server, $indent)
	{
		global $class_postfix_base;
		$class_name = $server->name;
		$base_class_name = $class_name . $class_postfix_base;
		echo "\n${indent}class $class_name : public cerl::Connection, public ${base_class_name}\n";
		echo "${indent}{\n";		
		$indent2 = $indent . "\t";
		
		echo "${indent}public:\n";	
		echo "${indent2}${class_name}()\n";
		echo "${indent2}{}\n\n";

		echo "${indent2}${class_name}(const cerl::Connection& conn)\n";
		echo "${indent2}\t: cerl::Connection(conn)\n";
		echo "${indent2}{}\n";

		$sentences = @$server->sentences;
		if ($sentences)
		{
			foreach ($sentences as $sent)
			{				
				$func = @$sent->function;
				if (!$func)
					continue;
				genFuncDef($sent->function, $indent2);
			}
		}
		echo "${indent}};\n";
	}
	
	function genSyncProxyClass($server, $indent)
	{
		global $class_postfix_base;
		$class_name = $server->name;
		$base_class_name = $class_name . $class_postfix_base;
		echo "\n${indent}class Sync$class_name : public ${base_class_name}\n";
		echo "${indent}{\n";		
		$indent2 = $indent . "\t";
		
		echo "${indent}private:\n";
		echo "${indent2}cerl::SyncConnection& m_conn;\n\n";
		
		echo "${indent}public:\n";
		echo "${indent2}Sync${class_name}(cerl::SyncConnection& conn)\n";
		echo "${indent2}\t: m_conn(conn)\n";
		echo "${indent2}{}\n";

		$sentences = @$server->sentences;
		if ($sentences)
		{
			foreach ($sentences as $sent)
			{				
				$func = @$sent->function;
				if (!$func)
					continue;
				genSyncFuncDef($sent->function, $indent2);
			}
		}
		echo "${indent}};\n";
	}
?>
<?php
	ob_start();
	
	$header = strtoupper("sdl_${module}_h");
	echo "/*\n";
	echo "\tDescription: 	Do not edit this file manually\n";	
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
	
	echo "\n#ifndef VENUS_CONNECTION_H\n";
	echo "#include <venus/Connection.h>\n";
	echo "#endif\n";

	echo "\n#pragma pack(1)\n";
	echo "\nnamespace $module {\n\n";
	
	foreach (@$doc->sentences as $sent)
	{
		$server = @$sent->server;
		if (!$server)
			continue;
		$current_server = $server->name;
		genProxyClass($server, "");
		genSyncProxyClass($server, "");
	}
	
	echo "\n} //namespace\n";
	echo "\n#pragma pack()\n";
	echo "\n#endif /* $header */ \n";
	
	$file_name = "sdl_${module}.h";
	$fd = fopen($file_name, 'w');
	fwrite($fd, ob_get_contents());
	fclose($fd);
	ob_clean();
	ob_end_flush();
?>