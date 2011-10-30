" Vimball Archiver by Charles E. Campbell, Jr., Ph.D.
UseVimball
finish
add_include.vim	[[[1
12
function Add_Include()
	let filename=expand("%:r") "获取
	let file_content=["EXEC SQL IFNDEF ".toupper(expand("%:r"))."_H;","EXEC SQL DEFINE ".toupper(expand("%:r"))."_H;"]
	let file_content+=getline(1,line('$'))
	let file_content+=["EXEC SQL ENDIF;"]
	let i=1
	for line in file_content
		call setline(i,line)
		let i=i+1
	endfor
endfunction

gotodef.vim	[[[1
104
"新窗口打开变量或函数定义
"具体用法
"	1.如果当前行是include语句 则调用Gotodef可以打开对应头文件
"	2.如果光标在宏函数上 则调用Gotodef可以打开对应宏定义
"	3.如果光标在函数名上 则调用Gotodef可以打开对应函数定义
function Gettoken() "获取当前光标所在的符号 可以是变量名 函数名 或者是文件名
	let curline=getline('.') "当前行
	let token_list=matchlist(curline,'^\s*EXEC\s\+SQL\s\+INCLUDE\s\+"\([^"]\+\)"\s*;\s*$\c') "EXEC INCLUDE指令
	if(len(token_list)==0)
		let token_list=matchlist(curline,'^\s*#include\s\+"\([^"]\+\)"\s*$') "INCLUDE宏 
	endif
	if(len(token_list)!=0)
		return [token_list[1],"head_file"]
	endif
	
	let validletter="[a-zA-Z0-9_]" "token合法字符
	let i=col('.') "当前列
	let j=i
	if match(curline[i-1],validletter)==-1
		return []
	endif
	while match(curline[i-1],validletter)!=-1
		let i=i-1
	endwhile	
	
	while match(curline[j-1],validletter)!=-1
		let j=j+1
	endwhile	
	let j=j-1
	let token=strpart(curline,i,j-i)
	if match(strpart(curline,j),'^\s*(')!=-1 "token是函数名
		return [token,"function"]
	elseif match(strpart(curline,j),'^\s*\.')!=-1 "token是结构体
		return [token,"struct"]
	endif
	return [token,"unknown"]
endfunction
function Gotodef() "打开当前光标所在的符号的定义
	let token_type=Gettoken()
	
	if(token_type==[])
		echo "非法符号"
		return
	endif
	
	let token=token_type[0]
	let type=token_type[1]
	
	if(type=="head_file") 
		let filepath=findfile(token,"/home/public/incl")	
		if(filepath=="") 
			let filepath=findfile(token,"/usr/include")	
		endif
		if(filepath !="")
			exec "sp ".filepath
		else
			echo "找不到头文件".token
		endif
	elseif(type=="function")
		let command=['grep ''^[^(]\+\<'.token.'\s*('' ', ' -n  |   grep -v ":\s*\*" |    grep -v ":\s*\/" | grep -v ";\s*$" | grep -v "=" |  grep -v ":\s*'.token.'"']
		let res=system(command[0].expand("%").command[1]) " 先搜本文件
		
		if(res !="")
			let line=split(res,"\n")
			let linenum=split(line[0],":")
			exec "sp "
			exec linenum[0]
		else
			if(res=="")
				let res=system(command[0].' -R /home/public/cgisrc/pub/  --include="*.ec" '.command[1]) " 再搜pub下.ec文件
			endif
			if(res=="")
				let res=system(command[0].' -R /home/public/cgisrc/pub/  --include="*.c" '.command[1]) " 再搜pub下.c文件
			endif
			if(res=="")
				let res=system('grep "^\s*#define\s*'.token.'\s*(" -R /home/public/incl/ -n --include="*.h"') "再搜宏定义
			endif
			if(res !="")
				let line=split(res,"\n")
				let file=split(line[0],":")
				exec "sp ".file[0] 
				exec file[1] 
			else
				echo "找不到函数定义"
			endif
		endif
	elseif(type=="struct")
		if filereadable("/home/public/incl/".token.".h")
			exec "sp /home/public/incl/".token.".h"
		else
			echo "找不到结构体定义"
		endif
	elseif(type=="unknown")
		if filereadable("/home/public/incl/".token.".h")
			exec "sp /home/public/incl/".token.".h"
		else
			echo "未知符号".token
		endif
	endif
endfunction




indent.vim	[[[1
463
"本脚本 处理源文件的缩进
"目前可以处理 c文件 ec文件 vim脚本 js文件
"by 陈源源


function Vim_indent(indent)
	let open_tokens=["function","function!","if","while","for"] "开始块
	let close_tokens=["endfor","endwhile","endif","endfunction"] "结束块
	let middle_tokens=['else','elseif'] "中间块
	let reserve_lines={} " 保留行 不缩进
	let indent_lines={} " 缩进行
	let file_content=getline(1,line('$'))
	let token_list=[]  " 处理缩进的数据结构 格式为[[type,linenum]...]
	
	" 缩进前处理一些特殊行
	if Prepare_indent(file_content,reserve_lines,indent_lines)  !=0
		return
	endif
	
	for i in sort(keys(indent_lines),"Sort_num")  
		for tokentype in Get_token(indent_lines[i],open_tokens,middle_tokens,close_tokens)
			call add(token_list,[tokentype,i])
		endfor
		let indent_lines[i]=""
	endfor
	
	let prevtype=token_list[0][0]
	let prevlinenum=token_list[0][1]
	let indent_lines[prevlinenum]=""  
	for i in  range(1,len(token_list)-1) 
		let type=token_list[i][0]
		let linenum=token_list[i][1]
		
		if(linenum==prevlinenum) " 同一行 缩进已经设置了 
			let prevtype=type
			let prevlinenum=linenum
			continue
		endif
		if prevtype==0  || prevtype==1 " 上一行是开始块或中间块
			for j in range(prevlinenum+1,linenum)
				let indent_lines[j]=indent_lines[prevlinenum].a:indent
			endfor
			if type !=0
				let indent_lines[linenum]=indent_lines[prevlinenum]
			endif
		else  " 上一行是结束块
			for j in range(prevlinenum+1,linenum)
				let indent_lines[j]=indent_lines[prevlinenum]
			endfor
			if type!=0 " 本行是中间块或结束块
				let indent_lines[linenum]=substitute(indent_lines[prevlinenum],a:indent,"","")
			endif
		endif
		let prevtype=type
		let prevlinenum=linenum
	endfor
	"缩进
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+1,total_lines[i])
	endfor

endfunction


function C_indent(indent,startindent,startline,endline)
	let open_tokens=["if","while","for"] "开始块
	let middle_tokens=['else\s\+if','else'] "中间块
	let reserve_lines={} " 保留行 不缩进
	let indent_lines={} " 缩进行
	let indent_lines2=[] " 缩进行
	let file_content=getline(a:startline,a:endline)
	
	" 缩进前处理一些特殊行
	if Prepare_indent(file_content,reserve_lines,indent_lines)  !=0
		return
	endif
	
	let j=0
	for i in sort(keys(indent_lines),"Sort_num")  
		call add(indent_lines2,substitute(indent_lines[i],'\(^\s\+\)\|\(\s\+$\)',"","g"))
		let indent_lines[i]=[j,""]
		let j+=1
	endfor
	
	
	
	let prevblank=""
	let notindent=0 " 下一行是否收缩
	for i in (range(1,len(indent_lines2)-1))
		let prevline=indent_lines2[i-1]
		let curline=indent_lines2[i]
		
		if curline[0]=='}' " 本行就是} 收缩
			"找出匹配的{
			let stack=1
			let flag=0
			for j in range(i-1,1,-1)
				let prevline=indent_lines2[j]
				for k in range(len(prevline)-1,0,-1)
					if prevline[k]=='}'
						let stack+=1
					elseif prevline[k]=='{'
						let stack-=1
						if(stack==0)
							for k in sort(keys(indent_lines),"Sort_num")  
								if indent_lines[k][0]==j
									let	curblank=indent_lines[k][1]
									let 	flag=1
									break
								endif
							endfor
							break
						endif
					endif
				endfor
				if flag==1
					break
				endif
			endfor
			if(stack!=0)
				for k in sort(keys(indent_lines),"Sort_num")  
					if indent_lines[k][0]==i
						echo "第".k+1."行的}找不到匹配的{ 缩进失败"
						return
					endif
				endfor
			endif
			let notindent=0
		elseif notindent==1
			let curblank=substitute(prevblank,a:indent,"","")
			let notindent=0
		elseif prevline =~ '{$' " 上一行是{
			"本行缩进一个tab
			let curblank=prevblank.a:indent
		elseif Token_match(prevline,open_tokens+middle_tokens)==1 " 上一行是 块开始标签
			if curline =~ '{$'   " 本行就是{ 无缩进
				let curblank=prevblank
			else "本行是普通语句
				let curblank=prevblank.a:indent
				let notindent=1 "下一行需要缩回
			endif
		elseif prevline[0]=='}'  " 上一行是}
			let curblank=prevblank
		else " 上一行是普通语句
			if Token_match(curline,middle_tokens)  " 本行就是 中间块 收缩
				if(match(prevline,'^\s*}') !=-1) " 上一行是}
					let curblank=prevblank
				else
					let curblank=substitute(prevblank,a:indent,"","")
				endif
			else "本行也是普通语句
				let curblank=prevblank
			endif
		endif
		let curline=substitute(curline,'^\s*',curblank,"")
		for k in sort(keys(indent_lines),"Sort_num")  
			if indent_lines[k][0]==i
				let indent_lines[k][1]=curblank
				let prevblank=curblank
				break
			endif
		endfor
	endfor
	
	
	
	
	for k in keys(indent_lines)
		let indent_lines[k]=a:startindent.indent_lines[k][1]
	endfor
	
	
	"缩进
	
	call Do_indent(file_content,reserve_lines,indent_lines)
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+a:startline,total_lines[i])
	endfor

endfunction



"获取line中的token
function Get_token(line,open_tokens,middle_tokens,close_tokens)
	let suffix=expand("%:p:e") "获取文件后缀
	if(suffix=="vim") 
		let tokens= split(substitute(a:line,'\([{}()]\)',' \1 ',"g"),' ')
		let token_list=[]
		let token=substitute(tokens[0],'\s*',"","g") " 去空格
		if index(a:open_tokens,token) >=0	
			let token_list+=[0]
		elseif index(a:middle_tokens,token) >=0
			let token_list+=[1]
		elseif index(a:close_tokens,token) >=0
			let token_list+=[2]
		endif
	endif
	return token_list
endfunction

"如果line的第一个token在token_list里面 返回1 否则返回 0
function Token_match(line,token_list)
	for token in a:token_list
		if(match(a:line,'^\s*'.token.'\>') !=-1)   
			return 1
		endif
	endfor
	return 0
endfunction

"缩进之前先剔除一些行 比如空格 注释之类的
function Prepare_indent(file_content,reserve_lines,indent_lines)
	let suffix=expand("%:p:e") "获取文件后缀
	if(suffix=="htm" || suffix=="html" ) " html文件
		let section_list=[]
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " 空行
				let a:reserve_lines[i]=-1
			elseif line =~ '^\s*<!-- @@[^@]\+@@ -->\s*$'
				let line=substitute(line,'\(^\s\+\)\|\(\s\+$\)',"","g") " 去前后空格
				let flag=0
				for j in range(len(section_list)-1,0,-1)
					let section=section_list[j]
					if section[1]==line	
						let a:reserve_lines[i]=section[0]  " 节点互相索引
						let a:reserve_lines[section[0]]=i  " 节点互相索引
						let flag=1
						call remove(section_list,j)
						break
					endif
				endfor
				if flag==0
					call add(section_list,[i,line])
				endif
			elseif match(line,'^\s*<!.*') !=-1  " html注释 或者dtd语句等 
				let a:reserve_lines[i]=-2
			else
				let a:indent_lines[i]=line
			endif
		endfor
		
		for j in range(len(section_list)-1,0,-1)
			echo "line ".j.": can not find match  section for ".section_list[j][1]
			return 1
		endfor
	elseif suffix=="c" || suffix=="ec" " c文件
		let commentline=-1
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " 空行
				let a:reserve_lines[i]=-1
			elseif line =~ '^\s*\/\*'  " 处理/*注释
				if commentline==-1
					let commentline=i
				else
					echo "line ".commentline.": can not find match  comment"
					return 1
				endif
			elseif line =~ '\*\/\s*$'  " 处理*/注释
				if commentline==-1
					echo "line ".i.": can not find match  comment"
					return 1
				else
					" 互相索引
					let a:reserve_lines[i]=commentline
					let a:reserve_lines[commentline]=i
					let commentline=-1
				endif
			elseif match(line,'^\s*\/\/') !=-1  " //注释
				let a:reserve_lines[i]=-2
			elseif commentline ==-1
				let a:indent_lines[i]=line
			endif
		endfor
	elseif suffix=="vim"  " vim脚本
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " 空行
				let a:reserve_lines[i]=-1
			elseif match(line,'^\s*"') !=-1  " //注释
				let a:reserve_lines[i]=-2
			else
				let a:indent_lines[i]=line
			endif
		endfor
	endif
	
	return 0
endfunction


function Reverse_num(i,j)
	return a:j*1-a:i*1
endfunction
function Sort_num(i,j)
	return a:i*1-a:j*1
endfunction

"实施实际的缩进
function Do_indent(file_content,reserve_lines,indent_lines)
	let suffix=expand("%:p:e") "获取文件后缀
	let total_lines={}
	for i in keys(a:indent_lines)
		let total_lines[i]=substitute(a:file_content[i],'^\s*',a:indent_lines[i],"")
	endfor	
	
	for i in sort(keys(a:reserve_lines),"Reverse_num")
		if a:reserve_lines[i] == -1 || a:reserve_lines[i] ==-2 
			if has_key(total_lines,i+1)
				let indent=matchlist(total_lines[i+1],'^\s*')[0] 
				let total_lines[i]=substitute(a:file_content[i],'^\s*',indent,"")
			else
				let total_lines[i]=substitute(a:file_content[i],'^\s*',"","")
			endif
		else
			if(suffix=="htm" || suffix=="html" ) " html文件
				if a:reserve_lines[i] > i " 开节点
					let indent=matchlist(total_lines[i+1],'^\s*')[0] 
					let j=a:reserve_lines[i]
					let total_lines[i]=substitute(a:file_content[i],'^\s*',indent,"")
					let total_lines[j]=substitute(a:file_content[j],'^\s*',indent,"")
				endif
			elseif suffix=="c" || suffix=="ec"
				if a:reserve_lines[i] < i "  */注释
					let indent=matchlist(total_lines[i+1],'^\s*')[0] 
					let j=a:reserve_lines[i]
					let total_lines[j]=substitute(a:file_content[j],'^\s*',indent,"") " 缩进 /*
					let total_lines[i]=substitute(a:file_content[i],'^\s*',indent,"") " 缩进 */
					" 缩进中间的注释
					if i!=j
						let previndent=Get_indent(a:file_content[j])
						for k in range(j+1,i-1)
							let total_lines[k]=substitute(a:file_content[k],previndent,indent,"") 
						endfor
					endif
				endif
			endif	
		endif	
	endfor	
	return total_lines
endfunction

" 获取line的缩进
function Get_indent(line)
	for i in range(len(a:line))
		if a:line[i]  !=" "  && a:line[i] != "\t"
			break
		endif
	endfor
	return strpart(a:line,0,i)
endfunction


"缩进html文件
function Html_indent(indent)
	let reserve_lines={} " 保留行
	let indent_lines={} " 缩进行
	let tag_list=[]  " 处理缩进的数据结构 格式为[[tag,type,linenum]...]
	let file_content=getline(1,line('$'))
	
	" 缩进前处理一些特殊行
	if Prepare_indent(file_content,reserve_lines,indent_lines)  !=0
		return
	endif
	
	for i in range(line('$'))
		if has_key(indent_lines,i)
			for tag in Get_htmltag(indent_lines[i],i)
				call add(tag_list,tag)
			endfor
			let indent_lines[i]=""
		endif
	endfor
	
	
	"补足标签
	let i=0
	while i < len(tag_list)
		if(tag_list[i][1]==1) " close tag
			let j=i-1
			while(j >=0)
				if(tag_list[j][1]==0) " open tag
					if(tag_list[j][0]==tag_list[i][0]) "找到匹配的close tag type变成保存彼此的索引
						let tag_list[i][1]=j
						let tag_list[j][1]=i
						break
					else
						let tag_list[j][1]=j " 自包含tag 
					endif
				endif
				let j -= 1
			endwhile
			if(j<0)
				echo "line ".(tag_list[i][2]+1).": can not find match tag for </".tag_list[i][0].">"
				return
			endif
		endif
		let i +=1
	endwhile
	let i=1
	while i < len(tag_list)
		let num1=tag_list[i][2]	
		let num2=tag_list[i-1][2]	
		if(num1==num2) " 同一行的标签 缩进不变
			let i+=1
			continue
		endif
		if tag_list[i][1] < i " close tag	
			let num3=tag_list[tag_list[i][1]][2]	
			let indent_lines[num1]=indent_lines[num3]
		elseif tag_list[i-1][1] > i-1 " open tag
			let indent_lines[num1]=indent_lines[num2].a:indent
		else
			let indent_lines[num1]=indent_lines[num2]
		endif
		let i+=1
	endwhile
	
	"缩进
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+1,total_lines[i])
	endfor
endfunction

"获取line中的htmltag
"返回列表，形式为[[tag1,type1],[tag2,type2]...] type 0-开标签 1-闭标签
function Get_htmltag(line,linenum)
	let a=-1
	let b=-1
	let tag=[]
	for i in range(strlen(a:line))
		if(a:line[i]=="<")
			if(match(a:line[i+1],'[a-zA-Z]') !=-1)
				let a=i+1
			elseif(a:line[i+1]=='/')
				let b=i+2
			endif
		elseif(a:line[i]=='>' || match(a:line[i],'\s') !=-1)
			if(a!=-1)
				call add(tag,[tolower(strpart(a:line,a,i-a)),0,a:linenum])
				let a=-1
			elseif(b!=-1)
				call	add(tag,[tolower(strpart(a:line,b,i-b)),1,a:linenum])
				let b=-1
			endif
		endif
	endfor
	return tag
endfunction
key_maps.vim	[[[1
75
"映射键
let s:ip=system("echo $SSH_CLIENT")
"查找
map <space> /
"设置编码
set encoding=gb2312
nmap .  :call Switch_case()<cr>

"切换窗口时更新按键
au BufEnter *  call Set_Fkeys()
function Set_Fkeys()
"说明一下 这种:call调用的是vim函数 这些都是在该文件夹下的脚本下定义的
":!调用的是shell命令 有些命令放在/home/public/tools下面 因为/home/public/tools已经被我加入 $PATH环境变量 所以可以直接调用里面的命令
	let l:suffix=expand("%:p:e") "获取文件后缀
	if(l:suffix=="c" || l:suffix=="ec")
		"从首行到末行以tab为单位缩进 速度很慢
		map <F1> :call C_indent("\t","",1,line('$'))<CR>
		"打开关联的模板
		map <F2> :call Split_src()<CR>
		"打开光标下的函数
		map <F3> :call Gotodef()<cr>
		"编译当前目录
		map <F4> :call Run_makeall()<CR>
		"编译当前文件
		map <F5> :call Run_makefile()<CR>
		"ctail是我根据tail命令的源码改写编译出来的 主要是增加-a -b选项 指定 源编码和目标编码 用于把 通讯返回的utf8报文转成gbk报文 避免看到一堆乱码的情况 ctail的源码在/home/public/tools/tail/下 用 gcc tail.c code_exchange.c -o ctail命令编译
		map <F6> :!ctail -a utf8 -b gbk -f /home/public/logs/cnnic_neterr.log <cr> 
		"查看cnolnicerr.log日志 并过滤当前ip
		map <F7> :!tail -f /home/public/logs/cnolnicerr.log \| grep $(echo $SSH_CLIENT \| cut -d ':' -f 4 \| cut -d ' ' -f 1) <cr>
		map <F8> :!tail -f /home/public/logs/cnolnicsql.log <cr>
		"编译当前批处理并在后台执行 然后打开日志
		map <F9> w<CR>:call Run_makefile()<CR><ESC>:!%:p:r.cgi &  if [ -f  ctail ]; then ctail -a utf8 -b gbk -f /home/public/logs/cnolnicerr.log; else  tail -f /home/public/logs/cnolnicerr.log; fi<CR>
		"常用替换
		map <F10> :call Substitute_tpl()<CR>
		"调用insert_update_statement.pl脚本，脚本在/home/public/tools
		map <F11> :!insert_update_statement.pl<cr>
		let g:Fkeys_msg="F1 缩进 F2 打开相关文件 F3 打开符号定义 F4 当前目录全部编译 F5 编译当前文件 F6 查看cnnic_neterr.log F7 查看 cnolnicerr.log F8 查看cnolnicsql.log F9 跑批处理 F10 常用替换 F11 生成更新语句 F12 帮助"
	
	elseif(l:suffix=="htm" || l:suffix=="html" || l:suffix=="tpl")
		"缩进html这个很好用 因为以前的模板都很乱
		"不过有个缺点就是js不会缩进 
		map <F1> :call Html_indent("  ")<CR>
		map <F2> :call Split_src()<CR>
		map <F3> :call Substitute_tpl()<CR>
		"重新计算语法配色
		map <F4> <esc>:syntax sync fromstart<cr>
		
		"jsl是网上找的一个js代码静态检查工具，不是很好用，但是聊胜于无
		"源代码在/home/public/tools/jsl-0.3.0/src
		map <F5> :! jsl   -conf /home/public/tools/jsl_conf -process  %<CR>
		
		"把标签中 name="domainname" 旁边加上 value="@@DOMAINNAME@@"
		map <F9> :s/name=\(["']\)\([^\1]*\)\1/name=\1\2\1 value=\1\@\@\U\2\@\@\1/g<CR>      
		let g:Fkeys_msg="F1 缩进 F2 打开相关文件 F4 修复颜色 F5 检测js错误 F9 替换value F12 帮助"
	elseif(l:suffix=="h")
		"头文件增加 #ifndef #def #endif指令
		map <F1> :call Add_Include()<cr>
		map <F3> :call Gotodef()<cr>
		let g:Fkeys_msg="F1 加入重复包含指令 F3 打开符号定义 F12 帮助"
	elseif(l:suffix=="vim")
		map <F1> :call Vim_indent("\t")<CR>
		let g:Fkeys_msg="F1 缩进 F12 帮助"
	elseif(l:suffix=="php")
		map <F1> :call Temp_map()<CR>
		let g:Fkeys_msg="F1 临时键映射 F12 帮助"
	else
		let g:Fkeys_msg="尚未绑定按键"
	endif
endfunction

function Show_Fkeys()
	echo  g:Fkeys_msg
endfunction
map <F12> :call Show_Fkeys()<CR>

open_relate_src.vim	[[[1
91
"""""""""""""""""""""""""""""""""""""""""""""""""""
"用法:显示当前文件相关的文件，让用户选择并且在新窗口打开
"具体用法
" 1.如果当前文件是模板，显示调用这个模板的ec文件和这个模板上包含的ec文件
" 2.如果当前文件是ec文件，则显示包含这个ec文件的模板和这个ec文件使用的模板
"by 陈源源
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"搜索相关的文件
function Find_src()
	let l:suffix=expand("%:p:e") "获取文件后缀
	let l:filelist=[]
	if(l:suffix=="c" || l:suffix=="ec") "当前文件是.ec
		"搜索模板文件
		let l:searchres=system('grep   -o ''"[^"]\+htm"'' '.expand("%").' | sed  -e ''s/"//g'' | sort | uniq  ')
		for l:file in split(l:searchres,'\n')
			let l:file=fnamemodify(l:file,":p:t")
			let l:file=system("find  /usr/local/apache/template   -name '".l:file."'")
			if(l:file=="")
				let l:file=system("find  /usr/local/apache/htdocs   -name '".l:file."'")
			endif
			let l:filelist=l:filelist+split(l:file,'\n')
		endfor

		"搜索相关的ec
		let l:file=expand("%:t:r")
		let l:file=substitute(l:file,'_in$',"","")
		let l:file=substitute(l:file,'_update$',"","")
		let l:file=substitute(l:file,'\d$',"","")
		let l:searchres=system('find '.expand("%:p:h")."  -regex '.*".l:file.".*ec'")
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif

		"搜索有调用到该cgi的模板
		let l:searchres=system('grep '.expand("%:t:r").'.cgi -R /usr/local/apache/template /usr/local/apache/htdocs/ --include="*.htm" -l | sort | uniq ')
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif
	elseif(l:suffix=="htm")  "htm文件
		"搜索调用到的cgi
		let l:searchres=system(' grep ''[a-zA-Z0-9_-]\+\.cgi'' -o '.expand("%")." | sed -e 's/\\.cgi/.ec/g' | sort | uniq ")
		for l:file in split(l:searchres,'\n')
			let l:file=fnamemodify(l:file,":p:t")
			let l:file=system("find /home/public/cgisrc -name '".l:file."'")
			let l:filelist=l:filelist+split(l:file,'\n')
		endfor
		"搜索有调用到该模板的程序
		let l:searchres=system('grep '.expand("%:t").' -R /home/public/cgisrc --include="*.ec" -l | sort | uniq ')
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif
	endif
	
	"排序 剔除重复项 和当前文件
	let l:filelist=sort(l:filelist)
	let l:i=0
	let l:curfile=expand("%:p") 
	while(l:i < len(l:filelist))
		if(l:i+1<len(l:filelist) && l:filelist[l:i]==l:filelist[l:i+1] )
			call remove(l:filelist,l:i)
		elseif(l:filelist[l:i]==l:curfile)
			call remove(l:filelist,l:i)
		else
			let l:i+=1
		endif
	endwhile
	return l:filelist
endfunction



"打开文件
function Split_src() 
	let l:filelist=Find_src()
	let l:len=len(l:filelist)
	let l:index=0
	if(l:len==0)
		echohl WarningMsg | echo "找不到相关文件" | echohl None
		return
	elseif(l:len>1)
		let l:index=Show_options("以下文件跟本文件有关，请选择一个打开",l:filelist)
		if(l:index==-1)
			return 
		endif
	endif
	let srcname=l:filelist[l:index]
	exec 'sp '.srcname
endfunction


run_make.vim	[[[1
40
""""""""""""""""""""""""""""""""""""""""""""""""""""""
"用法:运行makefile编译文件
"具体用法
" 1.如果当前文件在pub目录下
"	调用Run_makefile生成的目标文件是.o文件
"	调用Run_makeall会编译整个pub,并把生成的.so文件移到到/usr/lib
" 2.如果当前文件不在目录下
"	调用Run_makefile生成的目标文件是.ec文件
"	调用Run_makeall会编译整个目录
"by 陈源源
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



function Run_makefile()
        exec "w" 
        if match(expand("%:p"),'pub/') != -1 "在pub目录下 编译成.o文件
                if match(expand("%:p"),'\.ec$') != -1
                        exec "!rm  %:p:h/%:t:r.c;rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.o -j"
                else
                        exec "!rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.o -j"
                endif
        else "非pub目录编译成.cgi文件
                if match(expand("%:p"),'\.ec$') != -1
                        exec "!rm  %:p:h/%:t:r.c;rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.cgi -j"
                else
                        exec "!rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.cgi -j"
                endif
        endif
endfunction

function Run_makeall()
        if match(expand("%:p"),'/home/public/cgisrc/pub/') != -1 "在pub目录下 编译成.o文件
		exec 'w !cd /home/public/cgisrc/pub; make all -j ; make cp'
	else
		exec "w !make -C %:p:h  all" 
        endif
endfunction


selector.vim	[[[1
64
function Show_options(waring,options)
	"显示提示信息
	let l:index=0
	let l:len=len(a:options)
	while(1)
		call Echo_options(l:index,a:waring,a:options)
		let c = getchar()
		if(c=="\<Up>")
			if(l:index>0)
				let l:index=(l:index-1)
			endif
		elseif(c=="\<Down>")
			if(l:index<l:len-1)
				let l:index=(l:index+1)
			endif
		elseif(c==27)
			let l:index=-1
			break
		elseif(c==13)
			break
		endif
		redraw!
	endwhile
	redraw!
	return l:index
endfunction

function Echo_options(index,waring,options)
	if(a:waring!="")
		echohl WarningMsg | echo a:waring | echohl None
	endif
		
	let l:maxnum=10 " 最多显示选项数
	let l:len=len(a:options)-1
	if(a:index < l:maxnum) 
		let l:start_index=0
		let l:end_index=l:len
		if l:end_index>l:maxnum-1
			let l:end_index=l:maxnum-1
		endif
	else
		let l:start_index=a:index-l:maxnum+1
		let l:end_index=a:index
	endif
	for l:i in range(l:start_index,l:end_index)
		if(l:i == a:index)
			echohl Pmenu 
		else
			echohl  PmenuSel 
		endif
		if(type(a:options[l:i]) ==type({})) "字典
			for a:key in keys(a:options[l:i])
				echo a:key." : ".a:options[l:i][a:key]
			endfor
		else
			echo a:options[l:i] 
		endif
		echohl None
	endfor
	if(l:end_index < l:len)
		echohl WarningMsg | echo '-- More --' | echohl None
	endif
	echohl WarningMsg | echo '<Up> 上 <Down> 下 <Enter> 选中 <Esc> 取消' | echohl None
endfunction
substitute.vim	[[[1
45
"常用的替换
let s:subentrys=['执行下面全部替换']
let s:subentrys+=[{ '消除disp_message': '%s/sprintf(disp_message,\(.*\)\n\s*\(\(error\)\|\(succ\)\)_disp(disp_message);.*\n\(\s*return\>.*\)/\2_disp(\1\r\5/g| %s/strcpy(disp_message,\(.*\)\n\s*\(\(error\)\|\(succ\)\)_disp(disp_message);.*\n\(\s*return\>.*\)/\2_disp(\1\r\5/g   ' }]
let s:subentrys+=[{"处理field_check" : 'g/struct\s\+field_check/s/\[[0-9]\+\]/[]/'}]
let s:subentrys+=[{"处理check_assgin_comm": 'g/check_assgin_comm/s/&(\(.*\)\[0\]),[0-9]\+/\1,arry_len(\1)/'}]
let s:subentrys+=[{"消除tpl_set_field": 'g/tpl_set_field(/s/tpl_set_field\s*(\s*tpl\s*,\s*\("[^"]\+"\)\s*,\([^,]\+\),[^;]\+;/tpl_set_field_char(tpl,\1,\2);/g'}]
let s:subentrys+=[{"替换tpl_http_write": 'g/tpl_http_write/s/1/STDOUT_FILENO/'}]
let s:subentrys+=[{"修改main" : '/^int\s\+main\s*(\s*argc\s*,\s*argv\s*)\s*/,/argv;/s/.*/int main(int argc,char **argv)/g  | %s/\(int main(int argc,char \*\*argv)\n\)\+/\1/g'}]
let s:subentrys+=[{"倒入模板" : "%s/倒入模板/导入模板/g"}]
let s:subentrys+=[{"返回值": "%s/return[ (]*-1[ )]*;/return FAIL;/g"}]
let s:subentrys+=[{"返回值": "%s/return[ (]*0[ )]*;/return SUCC;/g"}]
let s:subentrys+=[{"清除parameter" : 'g/parameter/s/\(EXEC SQL BEGIN DECLARE SECTION;\)\|\(EXEC SQL END DECLARE SECTION;\)\|\(\<parameter \)//g'}]
let s:subentrys+=[{"替换agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<1\>/domainreg\.agenttype\1domain_agenttype_nsi'}]
let s:subentrys+=[{"替换agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<13\>/domainreg\.agenttype\1domain_agenttype_icann'}]
let s:subentrys+=[{"替换agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<3\>/domainreg\.agenttype\1domain_agenttype_webcc'}]
let s:subentrys+=[{"替换agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<1\>/domainreg\.agenttype\1domain_agenttype_nsi'}]
let s:subentrys+=[{"替换agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<0\>/domainreg\.agenttype\1domain_agenttype_china_channel'}]
let s:subentrys+=[{"替换sqlcode": '%s/\<sqlcode\>\([ !=]\+\)\<100\>/sqlcode\1NOTEXIST/g'}]
let s:subentrys+=[{"消除trans_back": '%s/trans_back(NOTDISPLAY)/trans_back_v60()/g'}]
let s:subentrys+=[{"替换setblank": '%s/if\s*(strlen\s*(\s*\([a-zA-Z0-9.]\+\)\s*)\s*==\s*0\s*)\s*\n\s*strcpy\s*(\s*\1\s*,\s*" "\s*)\s*;/setblank(\1);/g '}]
let s:subentrys+=[{"替换xml报文" : '%s/set_send_xml_int(send,"\(.*\)",\(.*\));/vect_strcat(send,"<\1>%d<\/\1>",\2);/g' }]
let s:subentrys+=[{"替换xml报文" : '%s/set_send_xml(send,"\(.*\)",\(.*\));/vect_strcat(send,"<\1>%s<\/\1>",\2);/g' }]

function Substitute_tpl()
	let l:index=Show_options("请选择以下替换",s:subentrys)
	if(l:index==-1)
		return 
	endif
	
	if(l:index !=0)
		let l:entry=s:subentrys[l:index]
		for l:key in keys(l:entry)
			exec l:entry[l:key]
		endfor
	else
		let l:subentrys=copy(s:subentrys)
		call remove(l:subentrys,0)
		for l:entry in l:subentrys
			for l:key in keys(l:entry)
				exec l:entry[l:key]
			endfor
		endfor
	endif
endfunction

switch_case.vim	[[[1
29
"把光标下的单词转大小写

function Switch_case()
	" 找出光标下的token
	let curline=getline('.')
	let validletter="[a-zA-Z0-9_]" "token合法字符
	let i=col('.') "当前列
	let j=i
	if match(curline[i-1],validletter)==-1
		return []
	endif
	while match(curline[i-1],validletter)!=-1
		let i=i-1
	endwhile	
	
	while match(curline[j-1],validletter)!=-1
		let j=j+1
	endwhile	
	let j=j-1

	if strpart(curline,i,j-i)[0] =~ '[a-z]'  " 第一个字符是小写
		call setline(line('.'),strpart(curline,0,i).toupper(strpart(curline,i,j-i)).strpart(curline,j))
	else
		call setline(line('.'),strpart(curline,0,i).tolower(strpart(curline,i,j-i)).strpart(curline,j))
	endif
	let curline=getline('.')
endfunction


temp_map.vim	[[[1
23
"常用的键映射
let s:subentrys=[ '执行下面全部键映射', {'替换行': 'map b pkddyy=='}]
function Temp_map()
	let l:index=Show_options("请选择以下键映射",s:subentrys)
	if(l:index==-1)
		return 
	endif
	
	if(l:index !=0)
		let l:entry=s:subentrys[l:index]
		for l:key in keys(l:entry)
			exec l:entry[l:key]
		endfor
	else
		let l:subentrys=copy(s:subentrys)
		call remove(l:subentrys,0)
		for l:entry in l:subentrys
			for l:key in keys(l:entry)
				exec l:entry[l:key]
			endfor
		endfor
	endif
endfunction
