" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.

"============================================================================
" General settings
"----------------------------------------------------------------------------
set nocompatible        " nocp:  turn off vi compatibility
set undolevels=1000     " ul:  lots and lots of undo
set history=50          " hi:  size of :command history
set modelines=20
set modeline            " ml:  Turn on modelines

set ai                  " always set autoindenting on
set backup              " keep a backup file
set ruler               " show the cursor position all the time
filetype on
syntax on

"============================================================================
" Colors
"----------------------------------------------------------------------------
set t_Sf=.[3%dm        " set foreground (?)
set t_Sb=.[4%dm        " set background (?)

"============================================================================
" Presentation
"----------------------------------------------------------------------------
set shortmess=aIoO      " sm:  really short messages, don't show intro
set showmode            " smd:  show the current input mode
set more                " more:  page on extended output
set errorbells          " eb:  ring bell on error messages
set novisualbell        " novb:  turn of visual bell
set noequalalways       " noea:  don't always keep windows at equal size
set splitbelow          " sb:  splitted window appears below current one

"============================================================================
" Statusline, Ruler
"----------------------------------------------------------------------------
set laststatus=2        " ls:  always put a status line
set statusline=%([%-n]%y%1*%m%*\ %f%R%)\ %=\ %(%l,%c%V\ %P\ [0x%02.2B]%)
hi User1 term=inverse,bold cterm=bold ctermfg=red ctermbg=white
set maxfuncdepth=1000   " Need more depth for sub names

"============================================================================
" Filename Autocompletion
"----------------------------------------------------------------------------
set wildchar=<Tab>          " wc:  tab does autocompletion
set wildmode=longest,list   " wim:  bash-style autocompletion
" wig:  when autocompleting, ignore certain files
set wildignore=*~,#*#,*.sw?,*.o,*.class,*.java.html,*.cgi.html,*.html.html,.viminfo,*.pdf,*.mp3

"============================================================================
" Search and Replace
"----------------------------------------------------------------------------
set incsearch           " is:  show partial matches as search is entered
set hlsearch            " hls:  highlight search patterns
set ignorecase          " Ignore case distinction when searching
set smartcase           " ... unless there are capitals in the search string.
"set nowrapscan          " Don't wrap to top of buffer when searching

"============================================================================
" Tab standards
"----------------------------------------------------------------------------
set softtabstop=4
set shiftwidth=4
set shiftround " Shift to the next round tab stop
set expandtab

" Make p in Visual mode replace the selected text with the "" register.
vnoremap p <Esc>:let current_reg = @"<CR>gvs<C-R>=current_reg<CR><Esc>

behave xterm
if &term == "xterm"
    let &term = "xtermc"

    " Restore the screen when we're exiting
    set rs
    set t_ti=.7.[r.[?47h 
    set t_te=.[?47l.8
endif

" Enable file type detection.
" Use the default filetype settings, so that mail gets 'tw' set to 72,
" 'cindent' is on in C files, etc.
" Also load indent files, to automatically do language-dependent indenting.
filetype plugin indent on

"============================================================================
" Autocommands
"----------------------------------------------------------------------------
" For all text files set 'textwidth' to 78 characters.
au FileType text setlocal tw=78

" *.t files are Perl test files
au FileType pl,pm,t set filetype=perl

au FileType perl set makeprg=perl\ -c\ %\ $*
au FileType perl set errorformat=%f:%l:%m
au FileType perl set autowrite
" au FileType perl call PerlMode()
au FileType perl :noremap K :!perldoc <cword> <bar><bar> perldoc -f <cword><cr>

" http://vimdoc.sourceforge.net/htmldoc/vimfaq.html
" 5.5. How do I configure Vim to open a file at the last edited location?
" Vim stores the cursor position of the last edited location for each buffer
" in the '"' register. You can use the following autocmd in your .vimrc or
" .gvimrc file to open a file at the last edited location:
au BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \   exe "normal g`\"" |
    \ endif

au FileType text call TextMode()
au FileType mail call TextMode()
au FileType vim  set iskeyword+=. iskeyword+=/ iskeyword+=~

" Perl-specific settings
let perl_fold=1
let perl_include_pod=1
" syntax color complex things like @{${"foo"}}
let perl_extended_vars = 1

" PHP-specifics
"let php_sql_query = 0
"let php_htmlInStrings = 1
"let php_baselib = 1
let g:PHP_default_indenting = 0
let g:PHP_vintage_case_default_indent = 1


au BufNewFile * silent! 0r ~/.vim/templates/%:e.tpl

set showmatch  " show matches on parens, bracketc, etc.
set matchpairs+=<:>

" Folding
set foldmethod=marker
set nofoldenable

set background=dark
set autoindent
set cindent

" Set visible tabs and trailing spaces
set list listchars=tab:\|-,trail:.
set backspace=indent,eol,start

" Stop the annoying behavior of leaving comments on far left
set fo+=r

" Don't do the auto-highlighting
" From :help matchparen
let loaded_matchparen=1

" Suffixes that get lower priority when doing tab completion for filenames.
" These are files we are not likely to want to edit or read.
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc,.ln

" Paths to check for loading files with "gf"
set path=.,./Include,./Class
" make tab in visual mode indent code
vmap <tab> >gv
vmap <s-tab> <gv

