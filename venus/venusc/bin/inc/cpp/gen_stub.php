<?php
	/*
		Generate Server Stubs of RPC
	*/
	function genCall($func, $indent)
	{
		global $current_server;
	
		$funcname = $func->name;
		$rettype = getRetType($funcname);
		$argstype = getArgsType($funcname);
		$args_set = parseArgs(@$func->args);
		
		if (count($args_set) == 0)
		{
			echo "${indent}cerl::NoArgs _args;\n";
			echo "${indent}(void) _args;\n";
		}
		else if (@$argstype)
		{
			echo "${indent}${argstype} _args;\n";
		}
		
		if (@$rettype)
		{
			echo "${indent}${rettype} _result = cerl::code_unknown_error;\n";
		}

		// decode args
		$arg_count = count($args_set);
		
		if ($arg_count)
		{		
			echo "${indent}const bool _fOk = cerl::getMailBody(_ar, _args);\n\n";
			echo "${indent}if (_fOk)\n";
			echo "${indent}{\n";
		}
		else
		{
			echo "${indent}NS_CERL_IO::check_vt_null(_ar);\n";
		}
		
		$indent2 = $arg_count ? ($indent . "\t") : $indent;
		echo "${indent2}CERL_DUMP_RECV_REQUEST(caller->getSelf(), this, _fid, _args);\n";
		echo "${indent2}${funcname}(caller, _result";
		
		if (count($args_set) == 1)
		{
			echo ", _args";
		}
		else
		{
			foreach($args_set as $var => $tp)
				echo ", _args.${var}";
		}
			
		echo ");\n";
		
		if ($arg_count)
		{
			echo "${indent}}\n";
			//echo "${indent}else\n";
			//echo "${indent}\t_result = cerl::code_format_error;\n";
		}
		echo "\n${indent}caller->dbg_reply(this, _result);\n";
	}
	
	function genCast($func, $indent)
	{
		global $current_server;
		
		$funcname = $func->name;
		$async = @$func->async;
		$args_set = parseArgs(@$func->args);
		
		// decode args
		$arg_count = count($args_set);
		
		if ($arg_count)
		{
			$argsType = getArgsType($funcname);
			echo "${indent}${argsType} _args;\n";			
			echo "${indent}const bool _fOk = cerl::getMailBody(_ar, _args);\n\n";			
			echo "${indent}CERL_ASSERT(_fOk && \"$current_server::_handle_cast - $funcname\");\n";
			echo "${indent}if (_fOk)\n";
		}		
		else
		{
			echo "${indent}NS_CERL_IO::check_vt_null(_ar);\n";
		}
		
		$indent2 = $arg_count ? ($indent . "\t") : $indent;
		
		echo "${indent2}${funcname}(caller";
		if ($arg_count == 1)
		{
			echo ", _args";
		}
		else
		{
			foreach($args_set as $var => $tp)
				echo ", _args.${var}";
		}
		
		echo ");\n";
	}
	
	function enumFuncs($server, $indent, $cast)
	{
		global $current_server;
		$sentences = @$server->sentences;
		if (!$sentences)
			return;
		
		$once = true;
		
		foreach ($sentences as $sent)
		{
			$func = @$sent->function;
			if (!$func)
				continue;
			
			$name = $func->name;
			$async = @$func->async;
			$id = $func->id;
			$indent2 = $indent . "\t";
			$indent3 = $indent2 . "\t";
			
			if ($cast)
			{
				if ($async)
				{
					if ($once)
					{
						$once = false;
						echo "${indent}switch (_fid)\n";
						echo "${indent}{\n";
					}
					echo "${indent}case code_${name}:\n";
					echo "${indent2}{\n";
					
						genCast($func, $indent3);
					
					echo "${indent2}}\n";
					echo "${indent2}break;\n";
				}
				else
				{
					continue;
				}
			}
			else
			{
				if ($async)
					continue;
					
				if ($once)
				{
					$once = false;
					echo "${indent}switch (_fid)\n";
					echo "${indent}{\n";
				}	
				echo "${indent}case code_${name}:\n";
				echo "${indent2}{\n";
				
					genCall($func, $indent3);
					
				echo "${indent2}}\n";
				echo "${indent2}break;\n";
			}
		}
		
		if (false == $once)
		{
			if (!$cast)
			{
				echo "${indent}default:\n";
				echo "${indent2}{\n";
				echo "${indent2}\tcerl::handle_call(this, caller, _fid, _ar);\n";
				echo "${indent2}}\n";
			}
			else
			{
				echo "${indent}default:\n";
				echo "${indent2}{\n";
				echo "${indent2}\tskipMailBody(_ar);\n";
				echo "${indent2}\tCERL_TRACE(\"cerl::handle_cast - Unknown FID: %.8X\", _fid);\n";
				echo "${indent2}}\n";				
			}
		}
		
		$indentx = $once ? $indent : $indent2;
		if($once)
		{
			if(!$cast)
			{
				echo "${indentx}cerl::handle_call(this, caller, _fid, _ar);\n";
				echo "${indentx}\tCERL_TRACE(\"cerl::handle_call - Unknown FID: %.8X\", _fid);\n";
			}
			else
			{
				echo "${indentx}\tskipMailBody(_ar);\n";
				echo "${indentx}\tCERL_TRACE(\"cerl::handle_cast - Unknown FID: %.8X\", _fid);\n";
			}
		}
		if (false == $once)
		{
			echo "${indent2}break;\n"; 
			echo "${indent}}\n";
		}
	}
	
	function genProcessHandle($indent)
	{
		$indent2 = $indent."\t";
		$indent3 = $indent2."\t";
		echo "\n${indent}template <class ArchiveT>";
		echo "\n${indent}void cerl_call _process(cerl::Caller* _caller, ArchiveT& _ar)";
		echo "\n${indent}{";
		echo "\n${indent2}Caller* const caller = static_cast<Caller*>(_caller);";
		echo "\n${indent2}cerl::FID fid = caller->getFid();";
		echo "\n${indent2}if (is_async_call(fid))\n${indent2}{";
		echo "\n${indent3}_handle_cast(caller, fid, _ar);";
		echo "\n${indent2}}\n${indent2}else\n${indent2}{";
		echo "\n${indent3}_handle_call(caller, fid, _ar);";
		echo "\n${indent2}}";
		echo "\n${indent}}\n";
	}

	function genHandleCall($server, $indent)
	{
		echo "\n${indent}template <class ArchiveT>";
		
		$indent2 = $indent . "\t";
		echo "\n";
		echo "${indent}void cerl_call _handle_call(Caller* caller, cerl::FID _fid, ArchiveT& _ar)\n";
		echo "${indent}{\n";
			enumFuncs($server, $indent2, false/*sync calls*/);
		echo "${indent}}\n";
	}

	function genHandleCast($server, $indent)
	{
		echo "\n${indent}template <class ArchiveT>";

		$indent2 = $indent . "\t";
		echo "\n";
		echo "{$indent}void cerl_call _handle_cast(Caller* caller, cerl::FID _fid, ArchiveT& _ar)\n";
		echo "${indent}{\n";
			enumFuncs($server, $indent2, true/*async cast*/);

		echo "${indent}}\n";
	}

/*	function genMain($server, $indent, $ctor, $th)
	{
		global $class_postfix_impl,$class_postfix_stub;
		
		$impl_class_name = $server->name . $class_postfix_impl;
		$stub_class_name = $server->name . $class_postfix_stub;
		echo "\n";
		echo "${indent}static int cerl_callback _main$th(cerl::Process* _me, cerl::Caller* caller)\n";
		echo "${indent}{\n";
			$indent2 = $indent . "\t"; 
			
			if (count($ctor))
			{
				foreach($ctor as $var => $tp)
				{
					$typename = mapType($tp, "");
					echo "${indent2}$typename $var;\n";
				}
				echo "${indent2}{\n";
					$indent3 = $indent2 . "\t";
					echo "${indent3}cerl::MailPtr _initParam(caller);\n";
					echo "${indent3}ArchiveT _ar(_me, caller);\n";
					echo "${indent3}bool _fOk = ";
					
					$index = 0;
					foreach($ctor as $var => $tp)
					{
						if ($index ++)
							echo "\n${indent3}\t\t&& ";
						echo "NS_CERL_IO::get(_ar, ${var})";
						
					}
					echo ";\n";
					echo "${indent3}CERL_ASSERT(_fOk && \"$stub_class_name::_main\");\n";
					echo "${indent3}if (!_fOk)\n";
						echo "${indent3}\treturn cerl::code_format_error;\n";
				echo "${indent2}}\n";
			}
			else
			{
				echo "${indent2}CERL_ASSERT(caller == NULL);\n";
			}
			
			echo "${indent2}${impl_class_name} _impl(_me";
			if (count($ctor))
			{
				foreach($ctor as $var => $tp)
					echo ", $var";
			}
			echo ");\n";
			
			echo "${indent2}return cerl::gen_server_run(_me, static_cast<${stub_class_name}*>(&_impl));\n";
		echo "${indent}}\n";
	}
	
	function genStart($server, $indent, $ctor, $th)
	{
		global $class_postfix_impl, $class_postfix_stub;
		$class_name = $server->name;
		$impl_class_name = $server->name . $class_postfix_impl;
		$stub_class_name = $server->name . $class_postfix_stub;
		echo "\n";
		echo "${indent}inline cerl::LocalProcess cerl_call ${impl_class_name}::_start(cerl::Caller* caller";
		
		if (count($ctor))
		{
			foreach($ctor as $var => $tp)
			{
				$typename = mapType($tp, "&");
				echo ", const $typename $var";
			}
		}
		echo ")\n";
		
		echo "${indent}{\n";
			$indent2 = $indent . "\t";
			if (count($ctor))
			{
				echo "${indent2}cerl::ArchiveT _ar;\n";

				foreach($ctor as $var => $tp)
					echo "${indent2}NS_CERL_IO::put(_ar, $var);\n"; 
				
				echo "${indent2}cerl::Caller* const caller = cerl_new_mail(caller, _ar.close());\n";
			}
			
			$param_arg = count($ctor) ? "caller" : "NULL";
			echo "${indent2}return_cerl_dbg_spawn(cerl_node(caller), ${stub_class_name}::_main$th, $param_arg, \"${class_name}\", ${stub_class_name}::_fid2name);\n";
		echo "${indent}}\n";
	}
*/
	function genRun($server, $indent)
	{
		global $class_postfix_stub, $class_postfix_impl;
		$indent2 = $indent."\t";
		$class_name = $server->name;
		echo "${indent}inline void cerl_call ${class_name}${class_postfix_impl}::_run(cerl::Fiber self, SOCKET socketListen)";
		echo "\n${indent}{\n";
		echo "${indent2}cerl::ServerRunner<${class_name}${class_postfix_stub}>::run(self, static_cast<${class_name}${class_postfix_stub}*>(this), socketListen);";
		echo "\n${indent}}\n";
	}
	
	function genStubClass($server, $indent)
	{
		global $class_postfix_stub, $class_postfix_impl;
		global $ctors;
		
		$class_name = $server->name;
		$stub_class_name = $class_name . $class_postfix_stub;
		echo "\n${indent}class $stub_class_name : public ${class_name}${class_postfix_impl}\n";
		echo "${indent}{\n";
		echo "${indent}public:";
		$indent2 = $indent . "\t";
		
		genProcessHandle($indent2);
		genHandleCast($server, $indent2);
		genHandleCall($server, $indent2);

		$ctors = parseCtors($server);
			
/*			if (count($ctors))
			{
				$nth = 0;
				foreach ($ctors as $ctor)
				{
					$postfix = ($nth == 0) ? "" : "$nth";
					genMain($server, $indent2, $ctor, $postfix);
					$nth ++;
				}
			}
			else
			{
				genMain($server, $indent2, NULL, "");
			}
*/
		echo "${indent}};\n";
	}
	
	$ctors ;
	$header = strtoupper("sdl_${module}_stub_h");
?>
<?php
	global $ctors;
	
	ob_start();
	foreach (@$doc->sentences as $sent)
	{
		$server = @$sent->server;
		if (!$server)
			continue;
		
		$svr_name = $server->name;
		$current_server = $svr_name;
		
		$header = strtoupper("${module}_${svr_name}_stub_h");
		
		echo "/*\n";
		echo "\tDescription: 	Do not edit this file manually\n";	
		echo "\tAuthor:			SDL Compiler\n";
		/*echo "\tDate:			";
		$datetime = time() + 28800; echo date("Y-M-jS H:i:s\n",$datetime);*/
		echo "*/\n\n";
		echo "#ifndef $header\n";
		echo "#define $header\n";
		$guard = strtoupper("${module}_${svr_name}_impl_h");
		echo "\n#ifndef $guard\n";
		echo "#include \"${svr_name}Impl.h\"\n";
		echo "#endif\n";

		echo "\nnamespace $module {\n";

		genStubClass($server, "");
		echo "\n";
		
/*		if (count($ctors))
		{
			$nth = 0;
			foreach($ctors as $ctor)
			{
				$postfix = ($nth == 0) ? "" : "$nth";
				genStart($server, "", $ctor, $postfix);
				$nth ++;
			}
			
		}
		else
		{
			genStart($server, "", NULL, "");
		}
*/
		genRun($server, "");
		
		echo "\n} //namespace\n";
		echo "\n#endif /* $header */ \n";
		
		$file_name = $svr_name . "Stub.h";
		$fd = fopen($file_name, 'w');
		fwrite($fd, ob_get_contents());
		fclose($fd);
		ob_clean();
	}
	ob_end_flush();
?>
