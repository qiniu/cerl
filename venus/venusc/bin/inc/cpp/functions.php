<?php
	function isRefType($builtin)
	{
		return $builtin == "cerl::String" || $builtin == "cerl::PInformation" || $builtin == "cerl::NInformation";
	}
	
	function isPOD($type)
	{
		global $pod_types;
		$name = $type->named_type->name;
		if (isset($type->array))
			return false;
		if (in_array($name, $pod_types))
			return true;
		return false;
	}
	
	function mapType($type, $ref)
	{
		global $typeset, $current_server, $module;
		global $builtin_types;
		
		$named_type = @$type->named_type;
		$array = @$type->array;
		
		if ($named_type)
		{
			$name = $named_type->name;
			$builtin = @$builtin_types[$name];
			
			if ($builtin)
				$name = $builtin;
			
			if ($array)
			{
				$size = @$array->size;
				if ($size)
					$name = "cerl::Array<$name, $size>";
				else
					$name = "cerl::BasicArray<$name>";
					
				return ($name .= $ref);
			}
			
			if (isRefType($builtin))
				return ($name .= $ref);
					
			$tp_name = "$current_server::$named_type->name";
			$tp = @$typeset[$tp_name];
			if (!$tp && $current_server  != $module)
			{
				$tp_name = "$module::$named_type->name";
				$tp = @$typeset[$tp_name];
			}
			
			if (!$builtin && !$tp)
				die("\n---ERROR: Type $named_type->name not defined!\n");
				
			if ($tp)
				return  ( @$tp["primary"] ? $named_type->name : ($named_type->name . $ref) );
						
			if ( !$builtin && !$array)
				$name .= $ref;
			return $name;
		}
		else
		{
			if ($array)
				die("\n---ERROR: Can't define an array of unnamed type!\n");
		}
	}
	
	function mapCodeName($code)
	{
		global $module, $current_server;
		global $codeset;
		global $builtin_codes;
		
		$code_name = NULL;
		$codeinfo = @$codeset["$current_server::$code"];
		
		if ($codeinfo)
		{
			$code_name = "$current_server" . "::code_" . $code;
			if ($current_server != $module)
				$code_name = "$module::" . $code_name;
		}
		else
		{
			$codeinfo = @$codeset["$module::$code"];
			if($codeinfo)
				$code_name = $module . "::code_" . $code;
		}
		
		if (!$code_name)
		{
			$code_name = @$builtin_codes[$code];
			if ($code_name)
				$code_name = "cerl::code_" . $code;
		}
		
		if (!$code_name)
		{
			die("\n---ERROR: Can not find code definition of  '$code' in server '$current_server'!");
		}
		
		return $code_name;
	}
	
	function getCodeValue($code)
	{
		global $module, $current_server;
		global $codeset;
		global $builtin_codes;
		
		$value = NULL;
		$codeinfo = @$codeset["$current_server::$code"];		
		if (!$codeinfo)
			$codeinfo = @$codeset["$module::$code"];
				
		if($codeinfo)
			$value = @$codeinfo['value'];
		
		if (!$value)
			$value = @$builtin_codes[$code];
		
		if(!$value)
			die("\n---ERROR: Can not find code value of  '$code' in server '$current_server'!");
		
		return $value;
	}
	
	function checkCodedType($coded_type, $name)
	{
		global $current_server;
		$items = $coded_type->items;
		$used = array();
		foreach ($items as $item)
		{
			$code = $item->code;
			$value = getCodeValue($code);
			if (@$used[$value])
				die("\n---ERROR: There are duplicated code values in coded type '$name' in namespace '$current_server'!\n");
			$used[$value] = 1;
		}
	}
	
	
	function parseCtors($server) // collect arg list of each constructor
	{
		$ctors = array();
		$sentences = @$server->sentences;
		if (!$sentences)
			return $ctors;
			
		foreach ($sentences as $sent)
		{
			$ctor = @$sent->ctor;			
			if (!$ctor)
				continue; 
				
			$arglist = parseArgs(@$ctor->args);
			$ctors[] = $arglist;
		}
		
		return $ctors;
	}
	
	// collect arg list of a func as key->val
	// notice that the key is the arg_name , not the arg_type
	// cause the type will be mapped to other name during use
	function parseArgs($args) 
	{
		$args_set = array();
		
		if ($args)
		{
			foreach ($args as $arg)
			{
				$type = @$arg->type;
				$args_set[@$arg->name] =  $type;
			}
		}
		return $args_set;
	}
	
	function processVars($vars, $indent)
	{
		if ($vars)
		{
			$indent .= "\t";
			foreach ($vars as $var)
			{
				echo $indent;
				$tp = mapType($var->type, "");
				echo "$tp $var->name;\n";
			}
		}
	}
	
	
	function getStructCount($items)
	{
		$count = 0;
		if ($items)
		{
			foreach ($items as $it)
			{
				$vars = @$it->vars;
				if ($vars)
					++$count;
			}
		}
		return $count;
	}
	
	function scanSent($sent, $indent, $codedef_sensitive)
	{
		global $typeset, $codeset, $current_server;
		
		$codedef = @$sent->codedef;
		if ($codedef)
		{
			if ($codedef_sensitive)
				defcode($codedef, $indent);
				
			return true;
		}
		else
		{
			$typedef = @$sent->typedef;
			if ($typedef)
			{
				if ($typedef->name == $current_server)
					die("---ERROR: Type '$typedef->name' is conflict with server name '$current_server'!");
				
				$tp_name = "$current_server::$typedef->name";
				if (@$typeset[$tp_name])
					die("---ERROR: Type '$typedef->name' has already been defined in namespace '$ns' !");
							
				echo "\n${indent}typedef ";
				$is_primary = deftype($typedef->type, $typedef->name, $indent);				
				$typeset[$tp_name] = array('ns' => $current_server, 'name' => $typedef->name, 'primary' => $is_primary);
				
				echo " $typedef->name;\n";
				return true;
			}
			return false;
		}
	}
	
	/*
		return a md5 string of a type definition
		only impled and applied in ret type process so far
	*/
	function traitType($type)
	{
		//TODO: if somebody just write type SomeTArray = SomeT[100];
		$named_type = @$type->named_type;
		assert(!$named_type);
		
		$coded_type = @$type->coded_type;
		
		$symbol_array = array();
		
		if ($coded_type)
		{
			$items = $coded_type->items;
			foreach ($items as $item)
			{
				$vars = @$item->vars;
				if(!$vars)
					continue;
				foreach($vars as $var)
				{
					$tp = $var->type->named_type->name;
					$name = $var->name;
					$symbol_array[] = $tp . $name;
				}
			}
		}
		else
		{
			$vars = @$type->struct->vars;
			if(!$vars)
				continue;
				
			foreach($vars as $var)
			{
				$tp = $var->type->named_type->name;
				$name = $var->name;
				$symbol_array[] = $tp . $name;
			}
		}

		sort($symbol_array);
		$str = "";
		foreach($symbol_array as $symbol)
			$str .= $symbol;
		
		return md5($str);
	}
	
	function isEnum($coded_type)
	{
		if ($coded_type)
		{
			$items = $coded_type->items;
			foreach ($items as $item)
			{
				if (@$item->vars)
					return false;
			}
			return true;
		}
		
		return false;
	}
	
	function push_pod_types($type, $typename)
	{
		global $pod_types;
		if (isset($type->struct))
		{
			foreach ($type->struct->vars as $var)
			{
				if (isset($var->type->array))
					return;
				if (in_array($var->type->named_type->name, $pod_types))
					continue;
				else
					return;
			}
			array_push($pod_types, $typename);
		}
		else
			if (in_array($type->named_type->name, $pod_types))
				array_push($pod_types, $typename);
	}
	
	/*
		return value: bool - is the type primary
	*/
	function deftype($type, $typename, $indent)
	{
		global $codeset, $typeset, $pod_types;
		global $module, $current_server;
		global $builtin_types, $derived_types;
		
		$named_type = @$type->named_type;
		$array = @$type->array;
		if ($named_type)
		{
			$name = $named_type->name;
			$builtin = @$builtin_types[$name];
			if ($builtin)
				$name = $builtin;
				
			if ($array)
			{
				$size = @$array->size;
				if ($size)
					$name = "cerl::Array<$name, $size>";
				else
					$name = "cerl::BasicArray<$name>";
				echo $name;
				return false;
			}
			
			if ($builtin == "cerl::String")
			{
				echo $name;
				return false;
			}
			
			push_pod_types($type, $typename);//向 pod_types 数组加入 pod 数据
			
			$tp_name = "$current_server::$named_type->name";
			$tp = @$typeset[$tp_name];
			if (!$tp && $current_server != $module)
			{
				$tp_name = "$module::$named_type->name";
				$tp = @$typeset[$tp_name];
			}
			
			if (!$builtin && !$tp)
				die("\n---ERROR: Type '$named_type->name' in '$current_server'not defined!\n");
				
			if ($tp)
			{
				echo $name;
				return @$tp["primary"];
			}
			
			echo $name;
			return true;
		}
		else /*non-named type*/
		{
			if ($array)
				die("ERROR: can't define an array of unnamed type.\n");
				
			$coded_type = @$type->coded_type;
			if ($coded_type)
				checkCodedType($coded_type, $typename);
				
			if (isEnum($coded_type))
			{
				echo "cerl::Code";
				return true; // means primary type
			}
			
			$exist_tp = typedefExistsType($type, $typename);
			if ($exist_tp)
			{
				echo $exist_tp;
				return false; // assume that traited types always non-primary
			}
			push_pod_types($type, $typename);//向 pod_types 数组加入 pod 数据
			echo "struct {\n";
			if ($coded_type)
			{
				$items = @$coded_type->items;
				$count = getStructCount($items);
				$indent2 = $indent . "\t";
				echo "${indent2}cerl::Code _code;\n";
				
				$indent3 = $indent2;
				if ($count > 1)
				{
					$indent3 = $indent2 . "\t";
					echo "${indent2}union {\n";
				}
				
				foreach ($coded_type->items as $it)
				{
					$vars = @$it->vars;
					if ($vars)
					{
						echo "${indent3}struct {\n";
						processVars($vars, $indent3);
						$it_name = ($it->code == "ok") ? "" : " " . $it->code;
						echo "${indent3}}$it_name;\n";
					}
				}
				if ($count > 1)
					echo "${indent2}};\n"; // union
			}
			else /*Noraml Struct*/
			{
				processVars(@$type->struct->vars, $indent);
			}
			echo "${indent}}"; // struct
			return false; // means non-primary type
		}
	}
	
	function defcode($codedef, $indent)
	{
		global $builtin_codes, $defined_codes;
		global $module, $current_server;
		global $codeset, $typeset;
		
		$code_name = "$current_server::$codedef->name";
		$code = @$codeset[$code_name];
		$fExists = $code ? true : false;
			
		if ($fExists)
			die("\n---ERROR: Code '$codedef->name' has already been defined in namespace '$current_server' !");
		else if (@$builtin_codes[$codedef->name])
			die("\n---ERROR: Code '$codedef->name' is already a builtin code!");
		
		if ($codedef->value[0] >= '0' && $codedef->value[0] <= '9')
		{
			$codeset[$code_name] = 
				array('ns' => $current_server, 'name' => $codedef->name, 'value' => $codedef->value);
		}
		else
		{
			if ($val = @$builtin_codes[$codedef->value])
			{
				$codeset[$code_name] = 
					array('ns' => $current_server, 'name' => $codedef->name, 'value' => $val);
			}
			else if (($val = @$codeset[$codedef->value]))
			{
				$ns = @$val["ns"];
				if ($module == $ns || $ns == $current_server)
				{
					$real_val = @$val["value"];
					$codeset[$code_name] = 
						array('ns' => $current_server, 'name' => $codedef->name, 'value' => $real_val);
				}
			}
			else
			{
				die("\n---ERROR: '$codedef->value' is not defined while assign it to '$codedef->name' in '$current_server'!\n");
			}
		}
		
		echo "${indent}enum { code_$codedef->name = $codedef->value };\n";
	}

	function processCodes($server, $indent)
	{
		global $current_server, $codeset;
		$sentences = @$server->sentences;
		$func_array = array();
		if ($sentences)
		{
			$indent .= "\t";
			foreach ($sentences as $sent)
			{
				$func = @$sent->function;
				$codedef = @$sent->codedef;
				
				if ($func)
				{
					$async = @$func->async;
					if ($async)
						echo "${indent}enum { code_$func->name = $func->id | CERL_FID_ASYNC_MASK };\n";
					else
						echo "${indent}enum { code_$func->name = $func->id };\n";
					$code_name = "$current_server::$func->name";
					$codeset[$code_name] = array('ns' => $current_server, 'name' => $func->name, 'value' => $func->id);
					$func_array["code_$func->name"] = $func->name;
				}
				else if ($codedef)
				{
					defcode($codedef, $indent);
				}
			}
		}
		return $func_array;
	}
	
	function genFunc2Name($func_array, $indent, $servername)
	{
		echo "\n";
		$indent .= "\t";
		echo "#if defined(VENUS_DEBUG_H)\n";
		echo "${indent}static const char* cerl_call _fid2name(cerl::FID fid)\n";
		echo "${indent}{\n";
		$indent2 = $indent . "\t";
		echo "${indent2}static const cerl::Fid2Name g_tbl[] =\n";
		echo "${indent2}{\n";
		$indent3 = $indent2 . "\t";
		foreach ($func_array as $fid => $name)
		{
			echo "${indent3}{ $fid, \"$servername.$name\" },\n";
		}
		echo "${indent2}};\n";
		echo "${indent2}return cerl::fid2name(fid, g_tbl, countof(g_tbl));\n";
		echo "${indent}}\n";
		echo "#endif\n";
	}

	function getRetType($name)
	{
		$rettype = "_" . $name . "Result";
		$rettype[1] = strtoupper($rettype[1]);
		return $rettype;
	}
	
	function typedefExistsType($type, $name)
	{
		global $typetraits, $current_server;
		global $derived_types;
		$trait = traitType($type);
		$tp = @$typetraits[$trait];
		
		if ($tp)
		{
			$ns = $tp['ns'];
			$exists_tp = $tp['name'];
			
			if ($ns != $current_server)
				$exists_tp = "${ns}Base::" . $exists_tp; // cause all type definition are in base file
			
			$derived_types[$current_server][$name] = 1; 
			return $exists_tp;
		}
		else
		{
			$typetraits[$trait] = array('ns' => $current_server, 'name' => $name);
			return "";
		}
	}
	
	function processRetType($func, $indent)
	{
		$async = @$func->async;
		$ret = @$func->type;
		$rettype = getRetType($func->name);	
		$indent2 = $indent . "\t";
		
		if ($async || !$ret)
			return;
		
		global $module, $current_server;
		global $typetraits, $derived_types;
		global $builtin_codes, $codeset;
		global $cpp_keywords;
		
		$coded_type = @$ret->coded_type;
		if (!$coded_type)
			die("\n---Error: while define RetType of '$func->name', ret type must be coded!");
		checkCodedType($coded_type, $rettype); //ret type always coded.
		
		$exist_tp = typedefExistsType($ret, $rettype);
		if ($exist_tp)
		{
			echo "\n${indent}typedef $exist_tp $rettype;\n";
			return;
		}
		
		echo "\n${indent}typedef struct ${rettype}Tag {\n";
		echo "${indent2}cerl::Code _code;\n";
		$items = $coded_type->items;
		$indent3 = $indent2;
				
		//将$items分为$constructArray和$nonConstructArray，分别表示包含构造函数和不包含构造函数的集合。
		$constructArray = array();
		$nonConstructArray = array();
		foreach ($items as $item)
		{
			$vars = @$item->vars;
			$flag = false;//是否包含构造函数，true表示放在union外，false表示放在union内。
			if ($vars)
			{
				foreach($vars as $var)
				{
					$type = $var->type;
					$tp = @$type->named_type;
					if (!$tp)
						die("\n---ERROR: Can not find the type definition of '$var->name' in '$func->name'!");
					$type_name = mapType($type, "");
					if (!isPOD($type))//如果isPOD是true，表示需要放在在union结构体内，否则在union之外！
					{
						$flag = true;
						break;
					}
				}
				if ($flag)
					array_push($constructArray, $item);
				else
					array_push($nonConstructArray, $item);
			}
		}
		if (count($nonConstructArray) > 0)
		{
			$count = getStructCount($nonConstructArray);
			if ($count > 1)
			{
				$indent3 = $indent2 . "\t";
				echo "${indent2}union {\n";
			}
			foreach ($nonConstructArray as $item)
			{
				$vars = @$item->vars;
				if ($vars)
				{
					echo "${indent3}struct {\n";
					foreach($vars as $var)
					{
						$type = $var->type;
						$tp = @$type->named_type;
						if (!$tp)
							die("\n---ERROR: Can not find the type definition of '$var->name' in '$func->name'!");
						$type_name = mapType($type, "");
						echo "{$indent3}\t$type_name $var->name;\n";
					}
					
					$item_name = ($item->code == "ok") ? "" : $item->code;
					if ($item->code != "ok" && @$cpp_keywords[$item->code])
						$item_name =  "_" . $item_name;
					echo "{$indent3}}$item_name;\n";
				}
			}
			if ($count > 1)
				echo "${indent2}};\n";
		}
		
		if (count($constructArray) > 0)
		{
			foreach($constructArray as $item)
			{
				$vars = @$item->vars;
				$indent3 = $indent2;
				if ($vars)
				{
					if ($item->code != "ok")
					{
						echo "${indent2}struct {\n";
						$indent3 = $indent2 . "\t";
					}
					foreach($vars as $var)
					{
						$type = $var->type;
						$tp = @$type->named_type;
						if (!$tp)
							die("\n---ERROR: Can not find the type definition of '$var->name' in '$func->name'!");
						$type_name = mapType($type, "");
						echo "{$indent3}$type_name $var->name;\n";
					}
					if ($item->code != "ok")
					{
						$item_name = $item->code;
						if (@$cpp_keywords[$item->code])
							$item_name =  "_" . $item_name;
						echo "{$indent2}}$item_name;\n";
					}
				}
			}
		}
			
		echo "\n${indent2}${rettype}Tag(cerl::Code code = cerl::code_error) {\n";
		echo "${indent2}\t_code = code;\n";
		echo "${indent2}}\n";
		
		echo "\n${indent2}operator cerl::Code() const {\n";
			echo "${indent2}\treturn _code;\n";
		echo "${indent2}}\n";
		
		echo "\n${indent2}void cerl_call operator=(cerl::Code code) {\n";
			echo "${indent2}\t_code = code;\n";
		echo "${indent2}}\n";

		// generate dumpCode function
		echo "\n${indent2}template <class LogT>\n";
		echo "${indent2}void cerl_call dumpCode(LogT& log) const {\n";
		echo "${indent2}\tNS_CERL_IO::dumpCode(log, _code);\n";
		echo "${indent2}}\n";
		
		echo "${indent}} ${rettype};\n"; /*end of struct*/
	}
	
	function getArgsType($name)
	{
		$argstype = "_" . $name . "Args";
		$argstype[1] = strtoupper($argstype[1]);
		return $argstype;
	}
	
	function processArgType($func, $indent)
	{
		$name = $func->name;
		$args_set = parseArgs(@$func->args);
		$i = count($args_set);
		if ($i == 0)
			return;
		else if ($i == 1)
		{
			foreach ($args_set as $var => $type)
			{
				$typename = mapType($type, "");
				$argsTyName = getArgsType($name);
				echo "\n${indent}typedef ${typename} $argsTyName;\n";
			}
		}
		else
		{
			echo "\n${indent}typedef struct {\n";
			foreach ($args_set as $var => $type)
			{
				$typename = mapType($type, "");
				echo "${indent}\t${typename} ${var};\n";
			}
			$argsTyName = getArgsType($name);
			echo "${indent}} $argsTyName;\n";
		}
	}

	function processArgTypes($server, $indent)
	{	
		$sentences = @$server->sentences;
		if ($sentences)
		{
			$indent .= "\t";
			foreach ($sentences as $sent)
			{
				$func = @$sent->function;
				if ($func)
					processArgType($func, $indent);
			}
		}
	}

	function processRetTypes($server, $indent)
	{
		$sentences = @$server->sentences;
		if ($sentences)
		{
			$indent .= "\t";
			foreach ($sentences as $sent)
			{
				$func = @$sent->function;
				if ($func)
					processRetType($func, $indent);
			}
		}
	}
	
	function typedefSent($sent)
	{
		$typedef = @$sent->typedef;
		if ($typedef)
			return true;
		return false;
	}
	
	$module = strtolower($doc->module);
	$current_server = $module; // module name or server name
	
	$builtin_types = array(
		"UInt32" => "cerl::UInt32",
		"Int32" => "cerl::Int32",
		"UInt64" => "cerl::UInt64",
		"Int64" => "cerl::Int64",
		"String" => "cerl::String",
		"Char" => "cerl::Char",
		"Byte" => "cerl::Byte",
		"Bool" => "cerl::Bool",
		"UInt16" => "cerl::UInt16",
		"Int16" => "cerl::Int16",
		"UInt8" => "cerl::UInt8",
		"Int8" => "cerl::Int8",
		"NInformation" => "cerl::NInformation",
		"PInformation" => "cerl::PInformation",
	);
	
	$pod_types = array("UInt32", "Int32", "UInt64", "Int64", "Char", "Byte", "Bool",
		"UInt16", "Int16", "UInt8", "Int8", "NInformation");
	
	$builtin_codes = array(
		"ok" => "0x0002",
		"error" => "0xffff",
		"true" => "0x0001",
		"false" => "0x0000",
	);
	
	$cpp_keywords = array(
		"true" => true,
		"false" => true,
		"inline" => true,
		"return" => true
	);
	
	
	// server_scoped actually;
	$non_defined_codes = array(
		/*
		code_name => svr_name;
		*/
	);
	
	$typeset = array(
		/*
		type_name => array('ns' => "svr_name" | "module_name", 'name' = "xxx", 'primary' => true | false)
		type_name = ns::name;
		*/
	);
	
	$codeset = array(
		/*
		code_name => array('ns' => "svr_name" | "module_name", 'name' = "xxx", 'value' => 0x11 )
		code_name = ns::name;
		*/
	);
	
	$typetraits = array(
		/*
		md5 => array('ns' => "svr_name" | "module_name", 'name' = "type_nmae")
		*/
	);
	
	$derived_types = array(
		/*
		ns => array(type_name => 1, ...);
		'ns' => "svr_name" | "module_name"
		*/
	);

	$class_postfix_impl = "Impl";
	$class_postfix_stub = "Stub";
	$class_postfix_base = "Base";
?>