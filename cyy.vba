" Vimball Archiver by Charles E. Campbell, Jr., Ph.D.
UseVimball
finish
add_include.vim	[[[1
12
function Add_Include()
	let filename=expand("%:r") "��ȡ
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
"�´��ڴ򿪱�����������
"�����÷�
"	1.�����ǰ����include��� �����Gotodef���Դ򿪶�Ӧͷ�ļ�
"	2.�������ں꺯���� �����Gotodef���Դ򿪶�Ӧ�궨��
"	3.�������ں������� �����Gotodef���Դ򿪶�Ӧ��������
function Gettoken() "��ȡ��ǰ������ڵķ��� �����Ǳ����� ������ �������ļ���
	let curline=getline('.') "��ǰ��
	let token_list=matchlist(curline,'^\s*EXEC\s\+SQL\s\+INCLUDE\s\+"\([^"]\+\)"\s*;\s*$\c') "EXEC INCLUDEָ��
	if(len(token_list)==0)
		let token_list=matchlist(curline,'^\s*#include\s\+"\([^"]\+\)"\s*$') "INCLUDE�� 
	endif
	if(len(token_list)!=0)
		return [token_list[1],"head_file"]
	endif
	
	let validletter="[a-zA-Z0-9_]" "token�Ϸ��ַ�
	let i=col('.') "��ǰ��
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
	if match(strpart(curline,j),'^\s*(')!=-1 "token�Ǻ�����
		return [token,"function"]
	elseif match(strpart(curline,j),'^\s*\.')!=-1 "token�ǽṹ��
		return [token,"struct"]
	endif
	return [token,"unknown"]
endfunction
function Gotodef() "�򿪵�ǰ������ڵķ��ŵĶ���
	let token_type=Gettoken()
	
	if(token_type==[])
		echo "�Ƿ�����"
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
			echo "�Ҳ���ͷ�ļ�".token
		endif
	elseif(type=="function")
		let command=['grep ''^[^(]\+\<'.token.'\s*('' ', ' -n  |   grep -v ":\s*\*" |    grep -v ":\s*\/" | grep -v ";\s*$" | grep -v "=" |  grep -v ":\s*'.token.'"']
		let res=system(command[0].expand("%").command[1]) " ���ѱ��ļ�
		
		if(res !="")
			let line=split(res,"\n")
			let linenum=split(line[0],":")
			exec "sp "
			exec linenum[0]
		else
			if(res=="")
				let res=system(command[0].' -R /home/public/cgisrc/pub/  --include="*.ec" '.command[1]) " ����pub��.ec�ļ�
			endif
			if(res=="")
				let res=system(command[0].' -R /home/public/cgisrc/pub/  --include="*.c" '.command[1]) " ����pub��.c�ļ�
			endif
			if(res=="")
				let res=system('grep "^\s*#define\s*'.token.'\s*(" -R /home/public/incl/ -n --include="*.h"') "���Ѻ궨��
			endif
			if(res !="")
				let line=split(res,"\n")
				let file=split(line[0],":")
				exec "sp ".file[0] 
				exec file[1] 
			else
				echo "�Ҳ�����������"
			endif
		endif
	elseif(type=="struct")
		if filereadable("/home/public/incl/".token.".h")
			exec "sp /home/public/incl/".token.".h"
		else
			echo "�Ҳ����ṹ�嶨��"
		endif
	elseif(type=="unknown")
		if filereadable("/home/public/incl/".token.".h")
			exec "sp /home/public/incl/".token.".h"
		else
			echo "δ֪����".token
		endif
	endif
endfunction




indent.vim	[[[1
463
"���ű� ����Դ�ļ�������
"Ŀǰ���Դ��� c�ļ� ec�ļ� vim�ű� js�ļ�
"by ��ԴԴ


function Vim_indent(indent)
	let open_tokens=["function","function!","if","while","for"] "��ʼ��
	let close_tokens=["endfor","endwhile","endif","endfunction"] "������
	let middle_tokens=['else','elseif'] "�м��
	let reserve_lines={} " ������ ������
	let indent_lines={} " ������
	let file_content=getline(1,line('$'))
	let token_list=[]  " �������������ݽṹ ��ʽΪ[[type,linenum]...]
	
	" ����ǰ����һЩ������
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
		
		if(linenum==prevlinenum) " ͬһ�� �����Ѿ������� 
			let prevtype=type
			let prevlinenum=linenum
			continue
		endif
		if prevtype==0  || prevtype==1 " ��һ���ǿ�ʼ����м��
			for j in range(prevlinenum+1,linenum)
				let indent_lines[j]=indent_lines[prevlinenum].a:indent
			endfor
			if type !=0
				let indent_lines[linenum]=indent_lines[prevlinenum]
			endif
		else  " ��һ���ǽ�����
			for j in range(prevlinenum+1,linenum)
				let indent_lines[j]=indent_lines[prevlinenum]
			endfor
			if type!=0 " �������м��������
				let indent_lines[linenum]=substitute(indent_lines[prevlinenum],a:indent,"","")
			endif
		endif
		let prevtype=type
		let prevlinenum=linenum
	endfor
	"����
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+1,total_lines[i])
	endfor

endfunction


function C_indent(indent,startindent,startline,endline)
	let open_tokens=["if","while","for"] "��ʼ��
	let middle_tokens=['else\s\+if','else'] "�м��
	let reserve_lines={} " ������ ������
	let indent_lines={} " ������
	let indent_lines2=[] " ������
	let file_content=getline(a:startline,a:endline)
	
	" ����ǰ����һЩ������
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
	let notindent=0 " ��һ���Ƿ�����
	for i in (range(1,len(indent_lines2)-1))
		let prevline=indent_lines2[i-1]
		let curline=indent_lines2[i]
		
		if curline[0]=='}' " ���о���} ����
			"�ҳ�ƥ���{
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
						echo "��".k+1."�е�}�Ҳ���ƥ���{ ����ʧ��"
						return
					endif
				endfor
			endif
			let notindent=0
		elseif notindent==1
			let curblank=substitute(prevblank,a:indent,"","")
			let notindent=0
		elseif prevline =~ '{$' " ��һ����{
			"��������һ��tab
			let curblank=prevblank.a:indent
		elseif Token_match(prevline,open_tokens+middle_tokens)==1 " ��һ���� �鿪ʼ��ǩ
			if curline =~ '{$'   " ���о���{ ������
				let curblank=prevblank
			else "��������ͨ���
				let curblank=prevblank.a:indent
				let notindent=1 "��һ����Ҫ����
			endif
		elseif prevline[0]=='}'  " ��һ����}
			let curblank=prevblank
		else " ��һ������ͨ���
			if Token_match(curline,middle_tokens)  " ���о��� �м�� ����
				if(match(prevline,'^\s*}') !=-1) " ��һ����}
					let curblank=prevblank
				else
					let curblank=substitute(prevblank,a:indent,"","")
				endif
			else "����Ҳ����ͨ���
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
	
	
	"����
	
	call Do_indent(file_content,reserve_lines,indent_lines)
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+a:startline,total_lines[i])
	endfor

endfunction



"��ȡline�е�token
function Get_token(line,open_tokens,middle_tokens,close_tokens)
	let suffix=expand("%:p:e") "��ȡ�ļ���׺
	if(suffix=="vim") 
		let tokens= split(substitute(a:line,'\([{}()]\)',' \1 ',"g"),' ')
		let token_list=[]
		let token=substitute(tokens[0],'\s*',"","g") " ȥ�ո�
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

"���line�ĵ�һ��token��token_list���� ����1 ���򷵻� 0
function Token_match(line,token_list)
	for token in a:token_list
		if(match(a:line,'^\s*'.token.'\>') !=-1)   
			return 1
		endif
	endfor
	return 0
endfunction

"����֮ǰ���޳�һЩ�� ����ո� ע��֮���
function Prepare_indent(file_content,reserve_lines,indent_lines)
	let suffix=expand("%:p:e") "��ȡ�ļ���׺
	if(suffix=="htm" || suffix=="html" ) " html�ļ�
		let section_list=[]
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " ����
				let a:reserve_lines[i]=-1
			elseif line =~ '^\s*<!-- @@[^@]\+@@ -->\s*$'
				let line=substitute(line,'\(^\s\+\)\|\(\s\+$\)',"","g") " ȥǰ��ո�
				let flag=0
				for j in range(len(section_list)-1,0,-1)
					let section=section_list[j]
					if section[1]==line	
						let a:reserve_lines[i]=section[0]  " �ڵ㻥������
						let a:reserve_lines[section[0]]=i  " �ڵ㻥������
						let flag=1
						call remove(section_list,j)
						break
					endif
				endfor
				if flag==0
					call add(section_list,[i,line])
				endif
			elseif match(line,'^\s*<!.*') !=-1  " htmlע�� ����dtd���� 
				let a:reserve_lines[i]=-2
			else
				let a:indent_lines[i]=line
			endif
		endfor
		
		for j in range(len(section_list)-1,0,-1)
			echo "line ".j.": can not find match  section for ".section_list[j][1]
			return 1
		endfor
	elseif suffix=="c" || suffix=="ec" " c�ļ�
		let commentline=-1
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " ����
				let a:reserve_lines[i]=-1
			elseif line =~ '^\s*\/\*'  " ����/*ע��
				if commentline==-1
					let commentline=i
				else
					echo "line ".commentline.": can not find match  comment"
					return 1
				endif
			elseif line =~ '\*\/\s*$'  " ����*/ע��
				if commentline==-1
					echo "line ".i.": can not find match  comment"
					return 1
				else
					" ��������
					let a:reserve_lines[i]=commentline
					let a:reserve_lines[commentline]=i
					let commentline=-1
				endif
			elseif match(line,'^\s*\/\/') !=-1  " //ע��
				let a:reserve_lines[i]=-2
			elseif commentline ==-1
				let a:indent_lines[i]=line
			endif
		endfor
	elseif suffix=="vim"  " vim�ű�
		for i in range(len(a:file_content))
			if has_key(a:reserve_lines,i)
				continue
			endif
			let line=a:file_content[i]
			if match(line,'^\s*$') !=-1  " ����
				let a:reserve_lines[i]=-1
			elseif match(line,'^\s*"') !=-1  " //ע��
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

"ʵʩʵ�ʵ�����
function Do_indent(file_content,reserve_lines,indent_lines)
	let suffix=expand("%:p:e") "��ȡ�ļ���׺
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
			if(suffix=="htm" || suffix=="html" ) " html�ļ�
				if a:reserve_lines[i] > i " ���ڵ�
					let indent=matchlist(total_lines[i+1],'^\s*')[0] 
					let j=a:reserve_lines[i]
					let total_lines[i]=substitute(a:file_content[i],'^\s*',indent,"")
					let total_lines[j]=substitute(a:file_content[j],'^\s*',indent,"")
				endif
			elseif suffix=="c" || suffix=="ec"
				if a:reserve_lines[i] < i "  */ע��
					let indent=matchlist(total_lines[i+1],'^\s*')[0] 
					let j=a:reserve_lines[i]
					let total_lines[j]=substitute(a:file_content[j],'^\s*',indent,"") " ���� /*
					let total_lines[i]=substitute(a:file_content[i],'^\s*',indent,"") " ���� */
					" �����м��ע��
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

" ��ȡline������
function Get_indent(line)
	for i in range(len(a:line))
		if a:line[i]  !=" "  && a:line[i] != "\t"
			break
		endif
	endfor
	return strpart(a:line,0,i)
endfunction


"����html�ļ�
function Html_indent(indent)
	let reserve_lines={} " ������
	let indent_lines={} " ������
	let tag_list=[]  " �������������ݽṹ ��ʽΪ[[tag,type,linenum]...]
	let file_content=getline(1,line('$'))
	
	" ����ǰ����һЩ������
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
	
	
	"�����ǩ
	let i=0
	while i < len(tag_list)
		if(tag_list[i][1]==1) " close tag
			let j=i-1
			while(j >=0)
				if(tag_list[j][1]==0) " open tag
					if(tag_list[j][0]==tag_list[i][0]) "�ҵ�ƥ���close tag type��ɱ���˴˵�����
						let tag_list[i][1]=j
						let tag_list[j][1]=i
						break
					else
						let tag_list[j][1]=j " �԰���tag 
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
		if(num1==num2) " ͬһ�еı�ǩ ��������
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
	
	"����
	let total_lines=Do_indent(file_content,reserve_lines,indent_lines)
	for i in  keys(total_lines) 
		call setline(i+1,total_lines[i])
	endfor
endfunction

"��ȡline�е�htmltag
"�����б���ʽΪ[[tag1,type1],[tag2,type2]...] type 0-����ǩ 1-�ձ�ǩ
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
"ӳ���
let s:ip=system("echo $SSH_CLIENT")
"����
map <space> /
"���ñ���
set encoding=gb2312
nmap .  :call Switch_case()<cr>

"�л�����ʱ���°���
au BufEnter *  call Set_Fkeys()
function Set_Fkeys()
"˵��һ�� ����:call���õ���vim���� ��Щ�����ڸ��ļ����µĽű��¶����
":!���õ���shell���� ��Щ�������/home/public/tools���� ��Ϊ/home/public/tools�Ѿ����Ҽ��� $PATH�������� ���Կ���ֱ�ӵ������������
	let l:suffix=expand("%:p:e") "��ȡ�ļ���׺
	if(l:suffix=="c" || l:suffix=="ec")
		"�����е�ĩ����tabΪ��λ���� �ٶȺ���
		map <F1> :call C_indent("\t","",1,line('$'))<CR>
		"�򿪹�����ģ��
		map <F2> :call Split_src()<CR>
		"�򿪹���µĺ���
		map <F3> :call Gotodef()<cr>
		"���뵱ǰĿ¼
		map <F4> :call Run_makeall()<CR>
		"���뵱ǰ�ļ�
		map <F5> :call Run_makefile()<CR>
		"ctail���Ҹ���tail�����Դ���д��������� ��Ҫ������-a -bѡ�� ָ�� Դ�����Ŀ����� ���ڰ� ͨѶ���ص�utf8����ת��gbk���� ���⿴��һ���������� ctail��Դ����/home/public/tools/tail/�� �� gcc tail.c code_exchange.c -o ctail�������
		map <F6> :!ctail -a utf8 -b gbk -f /home/public/logs/cnnic_neterr.log <cr> 
		"�鿴cnolnicerr.log��־ �����˵�ǰip
		map <F7> :!tail -f /home/public/logs/cnolnicerr.log \| grep $(echo $SSH_CLIENT \| cut -d ':' -f 4 \| cut -d ' ' -f 1) <cr>
		map <F8> :!tail -f /home/public/logs/cnolnicsql.log <cr>
		"���뵱ǰ�������ں�ִ̨�� Ȼ�����־
		map <F9> w<CR>:call Run_makefile()<CR><ESC>:!%:p:r.cgi &  if [ -f  ctail ]; then ctail -a utf8 -b gbk -f /home/public/logs/cnolnicerr.log; else  tail -f /home/public/logs/cnolnicerr.log; fi<CR>
		"�����滻
		map <F10> :call Substitute_tpl()<CR>
		"����insert_update_statement.pl�ű����ű���/home/public/tools
		map <F11> :!insert_update_statement.pl<cr>
		let g:Fkeys_msg="F1 ���� F2 ������ļ� F3 �򿪷��Ŷ��� F4 ��ǰĿ¼ȫ������ F5 ���뵱ǰ�ļ� F6 �鿴cnnic_neterr.log F7 �鿴 cnolnicerr.log F8 �鿴cnolnicsql.log F9 �������� F10 �����滻 F11 ���ɸ������ F12 ����"
	
	elseif(l:suffix=="htm" || l:suffix=="html" || l:suffix=="tpl")
		"����html����ܺ��� ��Ϊ��ǰ��ģ�嶼����
		"�����и�ȱ�����js�������� 
		map <F1> :call Html_indent("  ")<CR>
		map <F2> :call Split_src()<CR>
		map <F3> :call Substitute_tpl()<CR>
		"���¼����﷨��ɫ
		map <F4> <esc>:syntax sync fromstart<cr>
		
		"jsl�������ҵ�һ��js���뾲̬��鹤�ߣ����Ǻܺ��ã�������ʤ����
		"Դ������/home/public/tools/jsl-0.3.0/src
		map <F5> :! jsl   -conf /home/public/tools/jsl_conf -process  %<CR>
		
		"�ѱ�ǩ�� name="domainname" �Ա߼��� value="@@DOMAINNAME@@"
		map <F9> :s/name=\(["']\)\([^\1]*\)\1/name=\1\2\1 value=\1\@\@\U\2\@\@\1/g<CR>      
		let g:Fkeys_msg="F1 ���� F2 ������ļ� F4 �޸���ɫ F5 ���js���� F9 �滻value F12 ����"
	elseif(l:suffix=="h")
		"ͷ�ļ����� #ifndef #def #endifָ��
		map <F1> :call Add_Include()<cr>
		map <F3> :call Gotodef()<cr>
		let g:Fkeys_msg="F1 �����ظ�����ָ�� F3 �򿪷��Ŷ��� F12 ����"
	elseif(l:suffix=="vim")
		map <F1> :call Vim_indent("\t")<CR>
		let g:Fkeys_msg="F1 ���� F12 ����"
	elseif(l:suffix=="php")
		map <F1> :call Temp_map()<CR>
		let g:Fkeys_msg="F1 ��ʱ��ӳ�� F12 ����"
	else
		let g:Fkeys_msg="��δ�󶨰���"
	endif
endfunction

function Show_Fkeys()
	echo  g:Fkeys_msg
endfunction
map <F12> :call Show_Fkeys()<CR>

open_relate_src.vim	[[[1
91
"""""""""""""""""""""""""""""""""""""""""""""""""""
"�÷�:��ʾ��ǰ�ļ���ص��ļ������û�ѡ�������´��ڴ�
"�����÷�
" 1.�����ǰ�ļ���ģ�壬��ʾ�������ģ���ec�ļ������ģ���ϰ�����ec�ļ�
" 2.�����ǰ�ļ���ec�ļ�������ʾ�������ec�ļ���ģ������ec�ļ�ʹ�õ�ģ��
"by ��ԴԴ
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"������ص��ļ�
function Find_src()
	let l:suffix=expand("%:p:e") "��ȡ�ļ���׺
	let l:filelist=[]
	if(l:suffix=="c" || l:suffix=="ec") "��ǰ�ļ���.ec
		"����ģ���ļ�
		let l:searchres=system('grep   -o ''"[^"]\+htm"'' '.expand("%").' | sed  -e ''s/"//g'' | sort | uniq  ')
		for l:file in split(l:searchres,'\n')
			let l:file=fnamemodify(l:file,":p:t")
			let l:file=system("find  /usr/local/apache/template   -name '".l:file."'")
			if(l:file=="")
				let l:file=system("find  /usr/local/apache/htdocs   -name '".l:file."'")
			endif
			let l:filelist=l:filelist+split(l:file,'\n')
		endfor

		"������ص�ec
		let l:file=expand("%:t:r")
		let l:file=substitute(l:file,'_in$',"","")
		let l:file=substitute(l:file,'_update$',"","")
		let l:file=substitute(l:file,'\d$',"","")
		let l:searchres=system('find '.expand("%:p:h")."  -regex '.*".l:file.".*ec'")
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif

		"�����е��õ���cgi��ģ��
		let l:searchres=system('grep '.expand("%:t:r").'.cgi -R /usr/local/apache/template /usr/local/apache/htdocs/ --include="*.htm" -l | sort | uniq ')
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif
	elseif(l:suffix=="htm")  "htm�ļ�
		"�������õ���cgi
		let l:searchres=system(' grep ''[a-zA-Z0-9_-]\+\.cgi'' -o '.expand("%")." | sed -e 's/\\.cgi/.ec/g' | sort | uniq ")
		for l:file in split(l:searchres,'\n')
			let l:file=fnamemodify(l:file,":p:t")
			let l:file=system("find /home/public/cgisrc -name '".l:file."'")
			let l:filelist=l:filelist+split(l:file,'\n')
		endfor
		"�����е��õ���ģ��ĳ���
		let l:searchres=system('grep '.expand("%:t").' -R /home/public/cgisrc --include="*.ec" -l | sort | uniq ')
		if(l:searchres !="")
			let l:filelist=l:filelist+split(l:searchres,'\n')
		endif
	endif
	
	"���� �޳��ظ��� �͵�ǰ�ļ�
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



"���ļ�
function Split_src() 
	let l:filelist=Find_src()
	let l:len=len(l:filelist)
	let l:index=0
	if(l:len==0)
		echohl WarningMsg | echo "�Ҳ�������ļ�" | echohl None
		return
	elseif(l:len>1)
		let l:index=Show_options("�����ļ������ļ��йأ���ѡ��һ����",l:filelist)
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
"�÷�:����makefile�����ļ�
"�����÷�
" 1.�����ǰ�ļ���pubĿ¼��
"	����Run_makefile���ɵ�Ŀ���ļ���.o�ļ�
"	����Run_makeall���������pub,�������ɵ�.so�ļ��Ƶ���/usr/lib
" 2.�����ǰ�ļ�����Ŀ¼��
"	����Run_makefile���ɵ�Ŀ���ļ���.ec�ļ�
"	����Run_makeall���������Ŀ¼
"by ��ԴԴ
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



function Run_makefile()
        exec "w" 
        if match(expand("%:p"),'pub/') != -1 "��pubĿ¼�� �����.o�ļ�
                if match(expand("%:p"),'\.ec$') != -1
                        exec "!rm  %:p:h/%:t:r.c;rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.o -j"
                else
                        exec "!rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.o -j"
                endif
        else "��pubĿ¼�����.cgi�ļ�
                if match(expand("%:p"),'\.ec$') != -1
                        exec "!rm  %:p:h/%:t:r.c;rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.cgi -j"
                else
                        exec "!rm %:p:h/%:t:r.o;make -C %:p:h  %:t:r.cgi -j"
                endif
        endif
endfunction

function Run_makeall()
        if match(expand("%:p"),'/home/public/cgisrc/pub/') != -1 "��pubĿ¼�� �����.o�ļ�
		exec 'w !cd /home/public/cgisrc/pub; make all -j ; make cp'
	else
		exec "w !make -C %:p:h  all" 
        endif
endfunction


selector.vim	[[[1
64
function Show_options(waring,options)
	"��ʾ��ʾ��Ϣ
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
		
	let l:maxnum=10 " �����ʾѡ����
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
		if(type(a:options[l:i]) ==type({})) "�ֵ�
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
	echohl WarningMsg | echo '<Up> �� <Down> �� <Enter> ѡ�� <Esc> ȡ��' | echohl None
endfunction
substitute.vim	[[[1
45
"���õ��滻
let s:subentrys=['ִ������ȫ���滻']
let s:subentrys+=[{ '����disp_message': '%s/sprintf(disp_message,\(.*\)\n\s*\(\(error\)\|\(succ\)\)_disp(disp_message);.*\n\(\s*return\>.*\)/\2_disp(\1\r\5/g| %s/strcpy(disp_message,\(.*\)\n\s*\(\(error\)\|\(succ\)\)_disp(disp_message);.*\n\(\s*return\>.*\)/\2_disp(\1\r\5/g   ' }]
let s:subentrys+=[{"����field_check" : 'g/struct\s\+field_check/s/\[[0-9]\+\]/[]/'}]
let s:subentrys+=[{"����check_assgin_comm": 'g/check_assgin_comm/s/&(\(.*\)\[0\]),[0-9]\+/\1,arry_len(\1)/'}]
let s:subentrys+=[{"����tpl_set_field": 'g/tpl_set_field(/s/tpl_set_field\s*(\s*tpl\s*,\s*\("[^"]\+"\)\s*,\([^,]\+\),[^;]\+;/tpl_set_field_char(tpl,\1,\2);/g'}]
let s:subentrys+=[{"�滻tpl_http_write": 'g/tpl_http_write/s/1/STDOUT_FILENO/'}]
let s:subentrys+=[{"�޸�main" : '/^int\s\+main\s*(\s*argc\s*,\s*argv\s*)\s*/,/argv;/s/.*/int main(int argc,char **argv)/g  | %s/\(int main(int argc,char \*\*argv)\n\)\+/\1/g'}]
let s:subentrys+=[{"����ģ��" : "%s/����ģ��/����ģ��/g"}]
let s:subentrys+=[{"����ֵ": "%s/return[ (]*-1[ )]*;/return FAIL;/g"}]
let s:subentrys+=[{"����ֵ": "%s/return[ (]*0[ )]*;/return SUCC;/g"}]
let s:subentrys+=[{"���parameter" : 'g/parameter/s/\(EXEC SQL BEGIN DECLARE SECTION;\)\|\(EXEC SQL END DECLARE SECTION;\)\|\(\<parameter \)//g'}]
let s:subentrys+=[{"�滻agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<1\>/domainreg\.agenttype\1domain_agenttype_nsi'}]
let s:subentrys+=[{"�滻agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<13\>/domainreg\.agenttype\1domain_agenttype_icann'}]
let s:subentrys+=[{"�滻agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<3\>/domainreg\.agenttype\1domain_agenttype_webcc'}]
let s:subentrys+=[{"�滻agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<1\>/domainreg\.agenttype\1domain_agenttype_nsi'}]
let s:subentrys+=[{"�滻agenttype": '%s/domainreg\.agenttype\([ !=]\+\)\<0\>/domainreg\.agenttype\1domain_agenttype_china_channel'}]
let s:subentrys+=[{"�滻sqlcode": '%s/\<sqlcode\>\([ !=]\+\)\<100\>/sqlcode\1NOTEXIST/g'}]
let s:subentrys+=[{"����trans_back": '%s/trans_back(NOTDISPLAY)/trans_back_v60()/g'}]
let s:subentrys+=[{"�滻setblank": '%s/if\s*(strlen\s*(\s*\([a-zA-Z0-9.]\+\)\s*)\s*==\s*0\s*)\s*\n\s*strcpy\s*(\s*\1\s*,\s*" "\s*)\s*;/setblank(\1);/g '}]
let s:subentrys+=[{"�滻xml����" : '%s/set_send_xml_int(send,"\(.*\)",\(.*\));/vect_strcat(send,"<\1>%d<\/\1>",\2);/g' }]
let s:subentrys+=[{"�滻xml����" : '%s/set_send_xml(send,"\(.*\)",\(.*\));/vect_strcat(send,"<\1>%s<\/\1>",\2);/g' }]

function Substitute_tpl()
	let l:index=Show_options("��ѡ�������滻",s:subentrys)
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
"�ѹ���µĵ���ת��Сд

function Switch_case()
	" �ҳ�����µ�token
	let curline=getline('.')
	let validletter="[a-zA-Z0-9_]" "token�Ϸ��ַ�
	let i=col('.') "��ǰ��
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

	if strpart(curline,i,j-i)[0] =~ '[a-z]'  " ��һ���ַ���Сд
		call setline(line('.'),strpart(curline,0,i).toupper(strpart(curline,i,j-i)).strpart(curline,j))
	else
		call setline(line('.'),strpart(curline,0,i).tolower(strpart(curline,i,j-i)).strpart(curline,j))
	endif
	let curline=getline('.')
endfunction


temp_map.vim	[[[1
23
"���õļ�ӳ��
let s:subentrys=[ 'ִ������ȫ����ӳ��', {'�滻��': 'map b pkddyy=='}]
function Temp_map()
	let l:index=Show_options("��ѡ�����¼�ӳ��",s:subentrys)
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
