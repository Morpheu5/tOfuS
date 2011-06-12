	;; For a.out compatibility.

;%ifdef HAVE_ASM_UNDERSCORE
;%define	EXT_C(symbol) _ %+ symbol
;%else
%define EXT_C(symbol) symbol
;%endif
