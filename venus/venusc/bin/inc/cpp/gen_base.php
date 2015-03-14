<?php
	/*
		Generate Base Class of servers and proxys including namespace scoped code definitions
	*/
	//include 'functions.php';
	
	function putCodedItems($namespace, $items, $indent)
	{
		global $module;
		global $builtin_codes;
		global $typeset, $codeset;
		
		echo "${indent}NS_CERL_IO::put(os, val._code);\n";
		$flagNeedSwitch = 0;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				if(0 == $flagNeedSwitch++)
				{
					echo "${indent}switch(val._code)\n";
					echo "${indent}{\n";
				}
				echo "{$indent}case $code_name:\n";
				foreach($vars as $var)
				{
					$var_member = "";
					if ("ok" == $item->code)
						$var_member = "val.$var->name";
					else
						$var_member = "val.$item->code.$var->name";
						
					echo "{$indent}\tNS_CERL_IO::put(os, $var_member);\n";
				}
				echo "{$indent}\tbreak;\n";
			}
		}
		
		if ($flagNeedSwitch)
		{
			echo "${indent}default:\n";
			echo "${indent}\tbreak;\n";
			echo "${indent}}\n";
		}
	}
	
	function getCodedItems($namespace, $items, $indent)
	{
		global $module;
		global $builtin_codes;
		
		$indent2 = $indent;
		echo "${indent2}if (!NS_CERL_IO::get(is, val._code))\n";
		echo "${indent2}\treturn false;\n\n";
		
		$indent3 = $indent2 . "\t";
			
		// since we have check enum types , so this always need 'switch'
		echo "${indent2}switch(val._code)\n";
		echo "${indent2}{\n";
			
		$pure_codes = array();
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if (!$vars)
			{
				$pure_codes[] = $code_name;
				continue;
			}
			else
			{
				if (count($pure_codes)) // make neibor $pure_codes share "case xx:"
				{
					foreach($pure_codes as $pc)
						echo "{$indent2}case $pc:\n";
					echo "{$indent3}return true;\n";
					$pure_codes = array();
				}
		
				echo "{$indent2}case $code_name:\n";
				echo "{$indent3}return ";
		
				$index = 0;
				foreach($vars as $var)
				{			
					if ($index ++)
						echo "\n${indent3}\t&& ";
					$var_member = "";
					if ("ok" == $item->code)
						$var_member = "val.$var->name";
					else
						$var_member = "val.$item->code.$var->name";
					echo "NS_CERL_IO::get(is, $var_member)";
				}
				echo ";\n";
			}
		}
		
		// since we have check enum types , so this always need 'switch'	
		echo "${indent2}default:\n";
		echo "${indent2}\treturn false;\n";
		echo "${indent2}}\n";
	}
	
	function copyCodedItems($namespace, $items, $indent)
	{
		global $module;
		global $builtin_codes;
		global $typeset, $codeset;
		$ret = true;
		$flagNeedSwitch = 0;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				$flagNeedCase = 0;
				foreach($vars as $var)
				{
					$var_src = "";
					$var_dest = "";
					if ("ok" == $item->code)
					{
						$var_src = "src.$var->name";
						$var_dest = "dest.$var->name";
					}
					else
					{
						$var_src = "src.$item->code.$var->name";
						$var_dest = "dest.$item->code.$var->name";
					}
					if (!isPOD($var->type))
					{
						$ret = false;
						if (0 == $flagNeedSwitch++)
							echo "${indent}switch(src._code)\n${indent}{\n";
						if (0 == $flagNeedCase++)
							echo "{$indent}case $code_name:\n";
						echo "{$indent}\tNS_CERL_IO::copy(alloc, $var_src, $var_dest);\n";
					}
				}
				echo "{$indent}\tbreak;\n";
			}
		}
		
		if ($flagNeedSwitch)
		{
			echo "${indent}default:\n";
			echo "${indent}\tbreak;\n";
			echo "${indent}}\n";
		}
		return $ret;
	}
	
	function dumpCodedItems($namespace, $items, $indent)
	{
		global $module;
		global $builtin_codes;
		global $typeset, $codeset;
		
		echo "${indent}args.dumpCode(log);\n";
		
		$flagNeedSwitch = 0;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				if(0 == $flagNeedSwitch++)
				{
					echo "${indent}switch(args._code)\n";
					echo "${indent}{\n";
				}
				echo "{$indent}case $code_name:\n";
				foreach($vars as $var)
				{
					$var_member = "";
					if ("ok" == $item->code)
						$var_member = "args.$var->name";
					else
						$var_member = "args.$item->code.$var->name";
					echo "{$indent}\tNS_CERL_IO::dump(log, \", \");\n";
					echo "{$indent}\tNS_CERL_IO::dump(log, $var_member);\n";
				}
				echo "{$indent}\tbreak;\n";
			}
		}
		if ($flagNeedSwitch)
		{
			echo "${indent}default:\n";
			echo "${indent}\tbreak;\n";
			echo "${indent}}\n";
		}
	}
	
	function serializeUserType($namespace, $userdef, $indent)
	{
		global $module, $current_server, $derived_types;
		$type_type = @$userdef->type;
		$coded_type = @$type_type->coded_type;
		$struct = @$type_type->struct;
		
		if (!$coded_type && !$struct)
			return;
		if ($coded_type && isEnum($coded_type))
			return;
			
		$type_name = $userdef->name;
		if (@$derived_types[$current_server][$type_name])
			return;
			
		$indent2 = $indent . "\t";
		//put
		echo "${indent}\nCERL_IO_BEGIN_PUTTER(${namespace}::$type_name)\n";
		echo "${indent2}IoTypeTraits<${namespace}::$type_name>::putType(os);\n";
		if ($coded_type)
			putCodedItems($namespace,$coded_type->items, $indent2);
		else
		{
			$vars = $struct->vars;
			foreach ($vars as $var)
				echo "{$indent2}NS_CERL_IO::put(os, val.$var->name);\n";
		}
		echo "${indent}CERL_IO_END_PUTTER()\n";
		//get
		echo "\n";
		echo "${indent}\nCERL_IO_BEGIN_GETTER(${namespace}::$type_name)\n";
		echo "${indent2}if (!IoTypeTraits<${namespace}::$type_name>::getType(is))\n";
		echo "${indent2}\treturn false;\n";
		if($coded_type)
			getCodedItems($namespace,$coded_type->items, $indent2);
		else
		{
			$vars = $struct->vars;
			echo "${indent2}return ";
			$index = 0;
			foreach ($vars as $var)
			{
				if ($index ++)
					echo "\n${indent2}\t&& ";
				echo "NS_CERL_IO::get(is, val.$var->name)";
			}
			echo ";\n";
		}
		echo "${indent}CERL_IO_END_GETTER()\n";
		//copy
		//printPODTYPE();
		$podTrue = true;
		echo "\n${indent}template <class AllocT>\n";
		echo "${indent}inline void copy(AllocT& alloc, $namespace::$type_name& dest, const $namespace::$type_name& src)\n";
		echo "${indent}{\n";
		echo "${indent2}dest = src;\n";
		if ($coded_type)
			$podTrue = copyCodedItems($namespace,$coded_type->items, $indent2);
		else
		{
			$vars = $struct->vars;
			foreach ($vars as $var)
				if (!isPOD($var->type))
				{
					echo "{$indent2}NS_CERL_IO::copy(alloc, dest.$var->name, src.$var->name);\n";
					$podTrue = false;
				}
		}
		echo "${indent}}\n";
		if ($podTrue)
			echo "${indent}CERL_PODTYPE($namespace::$type_name, true);\n";
		else
			echo "${indent}CERL_PODTYPE($namespace::$type_name, false);\n";
		//dump
		if (@$type_type->array)
			return;//do not generate dump for array
		$vars = $struct->vars;
		echo "\ntemplate<class LogT>\n";
		echo "inline void dump(LogT& log, const $namespace::$type_name& args)\n";
		echo "{\n";
		echo "\tNS_CERL_IO::dump(log, '{');\n";
		if ($coded_type)
			dumpCodedItems($namespace,$coded_type->items, $indent2);
		else
		{
			$i = 0;
			foreach ($vars as $var)
			{
				if ($i++ == 0)
					echo "\tNS_CERL_IO::dump(log, args.$var->name);\n";
				else
				{
					echo "\tNS_CERL_IO::dump(log, \", \");\n";
					echo "\tNS_CERL_IO::dump(log, args.$var->name);\n";
				}
			}
		}
		echo "\tNS_CERL_IO::dump(log, '}');\n";
		echo "}\n";
	}
	
	// assume RetType always be coded_type
	function serializeRetType($namespace, $func, $indent)
	{
		global $module;
		global $builtin_codes;
		global $current_server, $derived_types;
		global $cpp_keywords;
		
		$type = @$func->type;
		$async = @$func->async;
		if($async)
		{
			if ($type)
				die("\nERROR: You are expecting the 'async' function '$func->name' return a value!\n");
			return;
		}
		else if (!$type)
			die("\nERROR: Are you sure you have given 'sync' function '$func->name' a return value?\n");
		
		$retTyName = getRetType($func->name);
		
		if (@$derived_types[$current_server][$retTyName])
			return;
		$items = $type->coded_type->items;
		//put
		echo "${indent}\nCERL_IO_BEGIN_PUTTER(${namespace}::$retTyName)\n";
		$indent2 = $indent . "\t";
		$indent3 = $indent2 . "\t";
		echo "${indent2}NS_CERL_IO::put(os, val._code);\n";
		$flagNeedSwitch = 0;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				if(0 == $flagNeedSwitch++)
				{
					echo "${indent2}switch(val._code)\n";
					echo "${indent2}{\n";
				}
				echo "{$indent2}case $code_name:\n";
		
				foreach($vars as $var)
				{
					$var_member = "";
					if ("ok" == $item->code)
						$var_member = "val.$var->name";
					else
					{
						$ns_code_name = $item->code;
						if (@$cpp_keywords[$item->code])
							$ns_code_name = "_" . $ns_code_name;
						$var_member = "val.$ns_code_name.$var->name";
					}
				
					echo "{$indent3}NS_CERL_IO::put(os, $var_member);\n";
				}
				echo "{$indent3}break;\n";
			}
		}
		if ($flagNeedSwitch)
			echo "${indent2}default:\n${indent3}break;\n${indent2}}\n";
		echo "${indent}CERL_IO_END_PUTTER()\n\n";
		//get
		echo "${indent}CERL_IO_BEGIN_GETTER(${namespace}::$retTyName)\n";
		
		$only_code = true;
		foreach($items as $item)
		{
			if(@$item->vars)
			{
				$only_code = false;
				break;
			}
		}
		
		if ($only_code)
		{
			echo "${indent2}return NS_CERL_IO::get(is, val._code);\n";
			echo "${indent}CERL_IO_END_GETTER()\n";
		}
		else
		{
			echo "${indent2}if (!NS_CERL_IO::get(is, val._code))\n";
			echo "${indent3}return false;\n";
			
			$flagNeedSwitch = 0;
			foreach($items as $item)
			{
				$code_name = mapCodeName($item->code);
				
				$vars = @$item->vars;
				if ($vars)
				{
					if(0 == $flagNeedSwitch++)
					{	
						echo "\n${indent2}switch(val._code)\n";
						echo "${indent2}{\n";
					}
					echo "{$indent2}case $code_name:\n";
					echo "{$indent3}return ";
					$index = 0;
					foreach($vars as $var)
					{							
						$var_member = "";
						if ("ok" == $item->code)
							$var_member = "val.$var->name";
						else
						{
							$ns_code_name = $item->code;
							if (@$cpp_keywords[$item->code])
								$ns_code_name = "_" . $ns_code_name;
							$var_member = "val.$ns_code_name.$var->name";
						}
						if ($index ++)
							echo "\n{$indent3}\t&& ";
						echo "NS_CERL_IO::get(is, $var_member)";
					}
					echo ";\n";
				}
			}
			if ($flagNeedSwitch)
			{
				echo "${indent2}default:\n";
				echo "${indent3}return true;\n";
				echo "${indent2}}\n";
			}
			else
				echo "${indent2}return true;\n";
			echo "${indent}CERL_IO_END_GETTER()\n";
		}
		//copy
		//printPODTYPE($items);
		$podTrue = true;
		echo "\n${indent}template <class AllocT>\n";
		echo "${indent}inline void copy(AllocT& alloc, $namespace::$retTyName& dest, const $namespace::$retTyName& src)\n";
		echo "${indent}{\n";
		//echo "${indent2}NS_CERL_IO::copy(alloc, dest._code, src._code);\n";
		echo "${indent2}dest = src;\n";
		$flagNeedSwitch = 0;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				$flagNeedCase = 0;
				foreach($vars as $var)
				{
					$var_des = "";
					$var_src = "";
					if ("ok" == $item->code)
					{
						$var_des = "dest.$var->name";
						$var_src = "src.$var->name";
					}
					else
					{
						$ns_code_name = $item->code;
						if (@$cpp_keywords[$item->code])
							$ns_code_name = "_" . $ns_code_name;
						$var_des = "dest.$ns_code_name.$var->name";
						$var_src = "src.$ns_code_name.$var->name";
					}
					if (!isPOD($var->type))
					{
						if (0 == $flagNeedSwitch++)
							echo "${indent2}switch(src._code)\n${indent2}{\n";
						if (0 == $flagNeedCase++)
							echo "{$indent2}case $code_name:\n";
						echo "{$indent3}NS_CERL_IO::copy(alloc, $var_des, $var_src);\n";
						$podTrue = false;
					}
				}
				if ($flagNeedCase)
					echo "{$indent3}break;\n";
			}
		}
		if ($flagNeedSwitch)
			echo "${indent2}default:\n${indent3}break;\n${indent2}}\n";
		echo "${indent}}\n";
		if ($podTrue)
			echo "${indent}CERL_PODTYPE($namespace::$retTyName, true);\n";
		else
			echo "${indent}CERL_PODTYPE($namespace::$retTyName, false);\n";
		//dump
		echo "\ntemplate <class LogT>\n";
		echo "inline void cerl_call dump(LogT& log, const $namespace::$retTyName& result)\n";
		echo "{\n";
		echo "\tNS_CERL_IO::dump(log, '{');\n";
		echo "\tresult.dumpCode(log);\n";
		$once = true;
		foreach($items as $item)
		{
			$code_name = mapCodeName($item->code);
			$vars = @$item->vars;
			if ($vars)
			{
				if ($once == true)
				{
					$once = false;
					echo "\tswitch (result._code)\n\t{\n";
				}
				echo "\tcase ${code_name}:\n";
				foreach($vars as $var)
				{
					$result_member = "";
					if ("ok" == $item->code)
						$result_member = "result.$var->name";
					else
					{
						$ns_code_name = $item->code;
						if (@$cpp_keywords[$item->code])
							$ns_code_name = "_" . $ns_code_name;
						$result_member = "result.$ns_code_name.$var->name";
					}
					echo "\t\tNS_CERL_IO::dump(log, \", \");\n";
					echo "\t\tNS_CERL_IO::dump(log, $result_member);\n";
				}
				echo "\t\tbreak;\n";
			}
		}
		if ($once == false)
		{
			echo "\tdefault:\n\t\tbreak;\n";
			echo "\t}\n"; //end of switch
		}
		echo "\tNS_CERL_IO::dump(log, '}');\n";
		echo "}\n";
	}
	
	function serializeArgsType($namespace, $func, $indent)
	{
		global $cpp_keywords, $derived_types, $typeset, $current_server;
		
		$args = @$func->args;
		//there is not args anymore
		if (!$args)
			return;
		
		$args_set = parseArgs(@$func->args);
		$argsTyName = getArgsType($func->name);
		if (count($args_set) == 1)
			return;
		$tp_name = "$current_server::$argsTyName";

		if (@$derived_types[$current_server][$argsTyName])
			return;
		//there are args here
		//put
		echo "${indent}\nCERL_IO_BEGIN_PUTTER(${namespace}::$argsTyName)\n";
		$indent2 = $indent . "\t";
				
		foreach($args as $arg)
		{			
			$var_member = "val.$arg->name";	
			echo "{$indent2}NS_CERL_IO::put(os, $var_member);\n";
		}
		echo "${indent}CERL_IO_END_PUTTER()\n";
		
		//get
		echo "\n";
		echo "${indent}CERL_IO_BEGIN_GETTER(${namespace}::$argsTyName)\n";
		$indent2 = $indent . "\t";
		$indent3 = $indent2 . "\t";
		
		echo "${indent2}return ";
		
		$i = 0;
		foreach($args as $arg)
		{			
			$var_member = "val.$arg->name";	
			if ($i++ == 0)
				echo "NS_CERL_IO::get(is, $var_member)";
			else
				echo "\n${indent3}&& NS_CERL_IO::get(is, $var_member)";
		}
		echo ";\n";		
		echo "${indent}CERL_IO_END_GETTER()\n";
		
		//copy
		//printPODTYPE($args);
		$podTrue = true;
		echo "\n${indent}template <class AllocT>\n";
		echo "${indent}inline void copy(AllocT& alloc, $namespace::$argsTyName& dest, const $namespace::$argsTyName& src)\n";
		echo "${indent}{\n";
		echo "{$indent2}dest = src;\n";
		foreach($args as $arg)
			if (!isPOD($arg->type))
			{
				echo "{$indent2}NS_CERL_IO::copy(alloc, dest.$arg->name, src.$arg->name);\n";
				$podTrue = false;
			}
		echo "${indent}}\n";
		if ($podTrue)
			echo "${indent}CERL_PODTYPE($namespace::$argsTyName, true);\n";
		else
			echo "${indent}CERL_PODTYPE($namespace::$argsTyName, false);\n";
		//dump
		echo "\ntemplate<class LogT>\n";
		echo "inline void dump(LogT& log, const $namespace::$argsTyName& args)\n";
		echo "{\n";
		echo "\tNS_CERL_IO::dump(log, '{');\n";

		$i = 0;
		foreach ($args as $var)
		{
			if ($i++ == 0)
				echo "\tNS_CERL_IO::dump(log, args.$var->name);\n";
			else
			{
				echo "\tNS_CERL_IO::dump(log, \", \");\n";
				echo "\tNS_CERL_IO::dump(log, args.$var->name);\n";
			}
		}
		echo "\tNS_CERL_IO::dump(log, '}');\n";
		echo "}\n";
	}
	
	function genBaseClass($server, $indent)
	{
		global $class_postfix_base;
		$server_name = @$server->name;
		if ($server_name)
		{
			$class_name = $server_name . $class_postfix_base;
			echo "\n${indent}class $class_name\n";
			echo "${indent}{\n";
			echo "${indent}public:\n";
			
			$indent2 = $indent . "\t";
			$sentences = @$server->sentences;
			if ($sentences)
			{
				$func_array = processCodes($server, $indent);
				genFunc2Name($func_array, $indent, $server_name);
				
				echo "\n// Generated declaration data structure of Argments\n";
				echo "//////////////////////////////////////////////////////////////////////////\n";
				processArgTypes($server, $indent);
				
				echo "\n// Generated declaration data structure of Result\n";
				echo "//////////////////////////////////////////////////////////////////////////\n";
				processRetTypes($server, $indent);
				
				foreach ($sentences as $sent)
				{
					if (!scanSent($sent, $indent2, false))
						continue;
				}
			}
			echo "${indent}};\n";
		}
	}
?>
<?php
	ob_start();
	$header = strtoupper("sdl_${module}_base_h");
	
	echo "/*\n";
	echo "\tDescription: 	Do not edit this file manually\n";	
	echo "\tAuthor:			SDL Compiler\n";
	echo "*/\n\n";
	echo "#ifndef $header\n";
	echo "#define $header\n";
	echo "\n#ifndef VENUS_IO_H\n";
	echo "#include <venus/Io.h>\n";
	echo "#endif\n";
	echo "\n#pragma pack(1)\n";
	echo "\nnamespace $module {\n\n";
	echo "enum { code_ok = cerl::code_ok };\n";
	echo "enum { code_error = cerl::code_error };\n";
	echo "enum { code_true = cerl::code_true };\n";
	echo "enum { code_false = cerl::code_false };\n";

	foreach (@$doc->sentences as $sent)
	{
		if (!scanSent($sent, "", true))
		{
			$server = $sent->server;
			$current_server = $server->name;
			$whole_name = "$module::$server->name";
			if (@$typeset[$whole_name])
				die("\n---ERROR: There's a type name conflict with the coming server named '$server->name'!");
			
			genBaseClass($server, "");
		}
	}
	
	echo "\n} //namespace\n";
	echo "\n#pragma pack()\n";
	echo "\nNS_CERL_IO_BEGIN\n";
	echo "\n// Generated serialization of user types\n";
	echo "//////////////////////////////////////////////////////////////////////////\n";
	
	foreach (@$doc->sentences as $docsent)
	{
		global $class_postfix_base;
		$namespace = $module;
		$current_server = $module;
		if (typedefSent($docsent))
		{
			serializeUserType($namespace, $docsent->typedef, "");//including serialize dump and copy
			continue;
		}
		else
		{
			$server = @$docsent->server;
			if (!$server)
				continue;

			$namespace .= "::";
			$namespace .= $server->name;
			$namespace .= $class_postfix_base;
			$sentences = @$server->sentences;
			$current_server = $server->name;
			
			if (!$sentences)
				continue;
			
			foreach ($sentences as $sent)
			{
				if (@$sent->ctor)
					continue;
				if (typedefSent($sent))
					serializeUserType($namespace, $sent->typedef, "");//including serialize dump and copy
				else
				{
					$func = @$sent->function;
					if ($func)
					{
						echo "\n// Generated Args serialization for function $func->name\n";
						echo "//////////////////////////////////////////////////////////////////////////\n";	
						serializeArgsType($namespace, $func, "");//including serialize dump and copy
						echo "\n// Generated Ret serialization for function $func->name\n";
						echo "//////////////////////////////////////////////////////////////////////////\n";	
						serializeRetType($namespace, $func, "");//including serialize dump and copy
					}
				}
			}		
		}
	}

	echo "\nNS_CERL_IO_END\n";
	echo "\n#ifndef VENUS_IO_INL\n";
	echo "#include <venus/Io.inl>\n";
	echo "#endif\n";
	echo "#endif /* $header */ \n";

	$file_name = "sdl_${module}_base.h";
	$fd = fopen($file_name, 'w');
	fwrite($fd, ob_get_contents());
	fclose($fd);

	ob_clean();
	ob_end_flush();
?>
