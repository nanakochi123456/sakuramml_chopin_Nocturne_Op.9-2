IF(TINY=1) {
	Expression.Frequency(2);
	Modulation.Frequency(8);
	System.TimeBase=96
} ELSE {
	Expression.Frequency(1);
	Modulation.Frequency(1);
	System.TimeBase=192
}


///////////////////////////////////////////////////////////////////////////////
// title:        "tempo.h" ver 1.1
// description:  TempoChange非互換高機能テンポチェンジ機能インクルードファイル(2015/05/28)
// keywords:     SAKURA Function     by ななこっち★ @nanakochi123456
// HP:           http://nano.daiba.cx/
///////////////////////////////////////////////////////////////////////////////
// tempo.h デバッグ機能付きテンポチェンジ
// （サクラに TempoChange がありますが、その高機能版です）
//
// Include前に、以下の変数を定義して下さい。
//
// ・Int DEBUG=0 // 1 でメトロノームがなります、2 でテンポチェンジ時にうるさい雑音が鳴ります
// 　（メトロノームは10000小節で終了します、4/4 を前提に作成されています。）
//
// ・Int TEMPORATIO
// 　ベーステンポの割合を％で設定します。（100がデフォルト。）
//
// ・Int TEMPODIFF
// 　ベーステンポを設定して、外部よりテンポを制御したい場合、TEMPODIFF に 整数値 (-～+)を設定します。
//
// ・Int IGNORETEMPO
// 　IGNORETEMPO=1 にすると、テンポチェンジを無効にします。
//
// #METORONOMEMML
// 　メトロノームのリズムMMLを設定できます。3拍子にする時等使用します。
// 　(M=表拍、m=裏拍)
//
//  ※テンポチェンジは、専用のトラックを作成することをお勧めします。
///////////////////////////////////////////////////////////////////////////////
// Function TmpChange(tempo, len)
// テンポをチェンジします。
// 前のテンポは自動的に読み込まれます
// 即時チェンジする場合には、lenを 0 にします。
// それ以外は、!2 !4. 等を指定します。
//
// Function TmpChangeR(tempo, len)
// テンポをチェンジして、len分の休符を置きます。
///////////////////////////////////////////////////////////////////////////////
// テストするときは、以下のコメントを全て外してください。
//Int TEST=1
//Int DEBUG
//Int TEMPORATIO=100
//Int IGNORETEMPO=0
//Int TEMPODIFF=0
///////////////////////////////////////////////////////////////////////////////

#METORONOMEMML={M8m8m8 m8m8m8 m8m8m8 m8m8m8}

IF(#METORONOMEMML="") {
	#METORONOMEMML={M4m4m4m4}
}

IF(TEST=1) {
	DEBUG=2
	Tempo 200
}

IF(DEBUG>=1) {
	IF(DEBUG=2) {
		#DEBUGMML1={Sub{@1v90V(127)'v50o5do6fo7a#o8d'}}
		#DEBUGMML2={Sub{@1v90V(127)'v50o5co6eo7g#o8c'}}
		#DEBUGMML3={Sub{@1v127V(127)'v80o4cdefgabo6cdefgab'}}
	}
	TR(10)
		$M{n(MetronomeBell),}
		$m{n(MetronomeClick),}
		[10000
			Rythm{#METORONOMEMML}
		]
}
Function TmpChangeR(tempo, len) {
	TmpChange(tempo, len)
	r%(len)
}

Function TmpChange(tempo, len) {
	IF(len=0) {
		IF(DEBUG=2) {
			#DEBUGMML1
		}
	} ELSE {
		IF(DEBUG=2) {
			#DEBUGMML2
		}
	}

	IF(len=0) {
		IF(!(IGNORETEMPO<>1)) {
			IF(TEMPORATIO=0) {
				Tempo=tempo+TEMPODIFF
			} ELSE {
				Tempo=(tempo*TEMPORATIO)/100+TEMPODIFF
			}
		}
	} ELSE {
		IF(!(IGNORETEMPO<>1)) {
			Int TT1=Tempo
			Int TT2
			IF(TEMPORATIO=0) {
				TT2=tempo+TEMPODIFF
			} ELSE {
				TT2=(tempo*TEMPORATIO)/100+TEMPODIFF
			}
			Int TTDIFF=(TT2 - TT1)
			Int TTSTEP=TTDIFF
			IF(TTSTEP < 0) {
				TTSTEP=-TTDIFF
			}
			Int TTR=len / TTSTEP
			For(Int I=1; I < TTSTEP; I++) {
				Sub {
					r%(I*TTR)
					IF(TTDIFF < 0) {
						TT1 = TT1 - 1;
					} ELSE {
						TT1 = TT1 + 1;
					}
					Tempo = TT1;
				}
			}
			Sub{
				r%(len)
				IF(TEMPORATIO=0) {
					Tempo=tempo+TEMPODIFF
				} ELSE {
					Tempo=tempo*TEMPORATIO/100+TEMPODIFF
				}
			}
		}
	}
}

IF(TEST=1) {
	TR(1)
		TmpChange(100,0)
		o5l8cdefgfed
		TmpChange(120,!2)
		o5l8cdefgfed
		TmpChange(160,!2)
		o5l8cdefgfed
		TmpChange(80,!2)
		o5l8cdefgfed
		TmpChange(50,!2)
		o5l8cdefgfed
		TmpChange(160,!4)
		o5l8cdefgfed
		TmpChange(50,!2)
		o5l8cdefgfedc1
}

///////////////////////////////////////////////////////////////////////////////
// title:        "loop.h" ver 1.0
// description:  Loopインクルードファイル(2015/06/02)
// keywords:     SAKURA Function     by ななこっち★ @nanakochi123456
// HP:           http://nano.daiba.cx/
///////////////////////////////////////////////////////////////////////////////
// loop.h 可変回数のループ
///////////////////////////////////////////////////////////////////////////////
// Function LoopPlay
// LoopPlay(mml string value[, loop count offset]) max 5 parms.
//
// Example
// Int LOOP=4
// Include(loop.h)
// #mml={
//    cde
// }
// LoopPlay(#mml)
// LoopPlay(#mml,#mml2,0)
///////////////////////////////////////////////////////////////////////////////
// テストする時は、以下のコメントを外してください。
//Int TEST=1
//Int LOOP=5
///////////////////////////////////////////////////////////////////////////////

Function LoopPlay(Str Mml1, Str Mml2, Int count) {
	Str _MML
	For (Int i = 1; i < LOOP + count; i++) {
		_MML=_MML + Mml1 + Mml2
	}
	_MML=_MML + Mml1
	_MML
}

IF(TEST=1) {
	曲名{"かえるの歌(test)"}
	作者{"ドイツ民謡"}
	#mmla={
		l4
		cdef
		edc4.r8
	}
	#mmlb={
		l4
		efga
		gfe4.r8
	}

	#mmlc={
		crcr
		crcr
		l8ccddeeff
		l4
		edc2
	}
	
	TR(1)
		o5
		@(Clarinet)
		q95
		#mmla
		LoopPlay(#mmlb,3)
		#mmlc

	TR(2)
		o6
		@(Violin)
		q95
		[2 r1]
		#mmla
		LoopPlay(#mmlb,2)
		#mmlc

	TR(3)
		o4
		q95
		@(FrenchHorn)
		[4 r1]
		#mmla
		LoopPlay(#mmlb,1)
		#mmlc

	TR(4)
		o3
		q95
		@(Contrabass)
		[6 r1]
		#mmla
		LoopPlay(#mmlb,0)
		#mmlc

	TR(5)
		o3
		q95
		@(Timpani)
		[8 r1]
		#mmla
		LoopPlay(#mmlb,-1)
		#mmlc

}

///////////////////////////////////////////////////////////////////////////////
// title:        "rnd.h" ver 1.00
// description:  単音、和音、ドラムをランダムな音量とタイミングで演奏するインクルードファイル(2017/02/06)
// keywords:     SAKURA Function     by ななこっち★ @nanonano773
// License:      GPL3
///////////////////////////////////////////////////////////////////////////////
// rnd.h メロディー、和音、ドラムをランダムなタイミングで演奏する
///////////////////////////////////////////////////////////////////////////////
// 詳細の取扱説明書は、ファイル下部へ
//
// Function RMDINIT(rmin, rmax, vmin, vmax)
// 休符乱数値の最小値、最大値、最小、最大音量設定する。両方0にすると、同一タイミングで演奏します。
// 
// Function RMDR(rmin, rmax)
// 休符乱数値の最小値、最大値の設定のみする
//
// Function RNDV(rmin, rmax)
// 音量乱数値の最小値、最大値の設定のみする。
// 同一の値を設定を両方に行うと、固定音量となる。
// rmaxを省略すると、rmin、rmax共に加算、または減算を行なう
//
// rmin, rmaxは与える休符値、vmin, vmaxは与える音量値
// Function RMD(mml)
// ランダムに演奏したいメロディーのMMLを入力する
// Function RMDENABLE(1 or 0)
// メロディーランダム演奏機能を有効/無効にする
///////////////////////////////////////////////////////////////////////////////
// Function RCDINIT(rmin,rmax,vmin,vmax,vdif)
// 乱数値の最小値、最大値を設定する。両方0にすると、同一タイミングで演奏します。
// vmin,vmaxは音量の最小値、最大値
// vdifは音量増加値(マイナス値を指定すると減少値)
//
// Function RCD(mml)
// ランダムに演奏したい和音のMMLを入力する
// Function RCOT(mml)
// ランダムに演奏したい単音のMMLを入力すると、オクターブで演奏する。
// Function RCDENABLE(1 or 0)
// 和音ランダム演奏機能を有効/無効にする
// Function RCDR(rmin, rmax)
// Function RCDV(vmin, vmax)
//
// RCDをSRCDに置き換えた同名関数があります。ケースバイケースで使い分けられます。
//
// RDINIT(ドラム音量,randvフラグ,randrフラグ,BDロテートフラグ,SDロテートフラグ,シンバルロテートフラグ)
// RDV(Int V)
// RDCresc(Int V)
// RandV(Int MIN, Int MAX)
// RandR(Int MIN, Int MAX)
// SyncR()
// #RDRUMSET1
///////////////////////////////////////////////////////////////////////////////
// Example
// Include(rnd.h)
// TR(1)
//   RMDINIT(53,513,90,110)
//   l4 RMD(ceg)
//   RMDV(5)
//   l8 RMD(<b>fg)
//   RMDV(-5)
//   l8 RMD(ceg)
// TR(2)
//   RCDINIT(53,513)
//   l4 RCD(ceg)
//   l8 RCD(<b>fg)
//   RCDINIT(53,513,90,100)
//   l8 RCD(ceg)
// TR(10)
//   RDINIT(110,1,1,1,1,1)
//   #RDRUMSET1
//   l8 [4 Rythm {'bh'h'sh''bh''bh'h'sh'h}]
///////////////////////////////////////////////////////////////////////////////

Int RMD_RMIN=192
Int RMD_RMAX=768
Int RMD_VMIN=95
Int RMD_VMAX=100
Int RMD_CRESC=0
Int RMD_ENABLE=1

Int RCD_MIN=192
Int RCD_MAX=768
Int RCD_VMIN=-1
Int RCD_VMAX=-1
Int RCD_CRESC=0
Int RCD_TMIN=-1
Int RCD_TMAX=-1
Int RCD_ENABLE=1

Int SRCD_MIN=192
Int SRCD_MAX=768
Int SRCD_VMIN=-1
Int SRCD_VMAX=-1
Int SRCD_CRESC=0
Int SRCD_TMIN=-1
Int SRCD_TMAX=-1
Int SRCD_ENABLE=1

Int BDROTATE=0
Int SDROTATE=0
Int CYMROTATE=0

Int BDINIT=0
Int CYMINIT=0
Int SDINIT=0

Int RANDV=0
Int RANDR=0

Int RDVOL=127

Function RMDENABLE(Int FLG) {
	RMD_ENABLE=FLG
}

Function RMDINIT(Int RMIN, Int RMAX, Int VMIN, Int VMAX) {
	RMD_RMIN=RMIN
	RMD_RMAX=RMAX
	RMD_VMIN=VMIN
	RMD_VMAX=VMAX
	RMD_CRESC=0
}

Function RMDCresc(Str STEP) {
	RMD_CRESC=STEP
}

Function RMDR(Int RMIN, Int RMAX) {
	RMD_RMIN=RMIN
	RMD_RMAX=RMAX
}

Function RMDV(Int VMIN, Int VMAX) {
	IF(VMAX=0) {
		Int TMPMIN=RMD_VMIN
		Int TMPMAX=RMD_VMAX
		TMPMIN=TMPMIN+VMIN
		TMPMAX=TMPMAX+VMIN
		IF(TMPMIN < 0) {
			TMPMIN=0
		}
		IF(TMPMIN > 127) {
			TMPMAX=127
		}
		IF(TMPMAX < 0) {
			TMPMAX=0
		}
		IF(TMPMAX > 127) {
			TMPMAX=127
		}
		RMD_VMIN=TMPMIN
		RMD_VMAX=TMPMAX
	} ELSE {
		RMD_VMIN=VMIN
		RMD_VMAX=VMAX
	}
}

Function RMD(Str MML) {
	Str BUF=""
	Str FIRST=""
	Str ML=""
	Str M
	Str R
	Str VL
	Str RRMML
	Str RMML
	Str FM=""
	Int C=0
	Str LenStr
	Int TOKEN=0
	Int TO=0
	BUF=""
	R=""
	IF(RMD_ENABLE=0) {
		BUF=MML
	} ELSE {
		For(Int i=1; MID(MML, i, 1)!=""; i++) {
			M=MID(MML, i, 1)
			IF((ASC(M)>=48 && ASC(M)<=57) || M="r"||M="."||M="^") {
				IF(TO>=1) {
					LenStr=LenStr+M
				}
				ML=ML+M
			} ELSE {
				IF((ASC(M)>=97 && ASC(M)<=103)) {
					TOKEN++
					TO=1
					FM=RRMML+VL+ML+RMML
					IF(TOKEN>1) {
						C++
						IF(FM!="") {
							BUF=BUF+"Sub{"+FM+"}r"+LenStr
						}
						TOKEN=0
					} ELSE {
						BUF=BUF+ML
					}
					LenStr=""
					ML=M
					IF(RMD_RMAX>0) {
						R=Random(RMD_RMIN, RMD_RMAX)
						IF(Random(0,10)<5) {
							RRMML="r-"+R
							RMML="r"+R
						} ELSE {
							RRMML="r"+R
							RMML="r-"+R
						}
					}
					VL="v"+Random(RMD_VMIN, RMD_VMAX)
					RMDV(RMD_CRESC,0)
				} ELSE {
					ML=ML+M
					IF(TO=1) {
						TO=2
					}
					IF((ASC(M)>=97 && ASC(M)<=103) || M="r" || (ASC(M)>=48 && ASC(M)<=57) || M="." ||M="^"|| M="-"||M="#") {
					} ELSE {
						TO=0
					}
				}
			}
		}
		FM=RRMML+VL+ML+RMML
		IF(FM!="") {
			BUF=BUF+"Sub{"+FM+"}r"+LenStr
		}
	}
	BUF
}

Function RCDENABLE(Int FLG) {
	RCD_ENABLE=FLG
}

Function SRCDENABLE(Int FLG) {
	SRCD_ENABLE=FLG
}

Function RCDINIT(Int MIN, Int MAX, Int VMIN, Int VMAX, Int VDIF) {
	RCD_MIN=MIN
	RCD_MAX=MAX
	IF(VMIN<>0 && VMAX<>0) {
		RCD_VMIN=VMIN
		RCD_VMAX=VMAX
		RCD_CRESC=VDIF
	} ELSE {
		RCD_VMIN=-1
		RCD_VMAX=-1
		RCD_CRESC=0
	}
}

Function RCDR(Int RMIN, Int RMAX) {
	RCD_RMIN=RMIN
	RCD_RMAX=RMAX
}

Function RCDV(Int VMIN, Int VMAX) {
	RCD_CRESC=0
	IF(VMAX=0) {
		Int TMPMIN=RCD_VMIN
		Int TMPMAX=RCD_VMAX
		TMPMIN=TMPMIN+VMIN
		TMPMAX=TMPMAX+VMIN
		IF(TMPMIN < 0) {
			TMPMIN=0
		}
		IF(TMPMIN > 127) {
			TMPMAX=127
		}
		IF(TMPMAX < 0) {
			TMPMAX=0
		}
		IF(TMPMAX > 127) {
			TMPMAX=127
		}
		RCD_VMIN=TMPMIN
		RCD_VMAX=TMPMAX
	} ELSE {
		RCD_VMIN=VMIN
		RCD_VMAX=VMAX
	}
}

Function RCD(Str MML) {
	Str BUF=""
	Str FIRST=""
	Str ML=""
	Str M
	Str R
	Str FM
	Int C=0
	Str VL
	IF(RCD_ENABLE=0) {
		BUF="'" + MML + "'"
	} ELSE {
		RCD_TMIN=RCD_VMIN
		RCD_TMAX=RCD_VMAX

		BUF=""
		R=""
		For(Int i=1; MID(MML, i, 1)!=""; i++) {
			M=MID(MML, i, 1)
			IF(DEBUG=1||TEST=1) {
				Print("Count "+ C + " Str="+M)
			}
			IF(M="a"||M="b"||M="c"||M="d"||M="e"||M="f"||M="g") {
				C++
			  	IF(C<2) {
					FIRST=ML
					ML=FM+M
					FM=""
					IF(RCD_TMIN>=0 && RCD_TMAX>=0) {
						VL="v"+Random(RCD_TMIN, RCD_TMAX)
						RCD_TMIN=RCD_TMIN+RCD_CRESC
						RCD_TMAX=RCD_TMAX+RCD_CRESC
						IF(RCD_TMIN<0) { RCD_TMIN=0 }
						IF(RCD_TMIN>127) { RCD_TMIN=127 }
						IF(RCD_TMAX<0) { RCD_TMAX=0 }
						IF(RCD_TMAX>127) { RCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+ML
				} ELSE {
					BUF=BUF+"Sub" + CHR(123)+R+ML+CHR(125)
					IF(RCD_MAX>0) {
						R=R+"r"+Random(RCD_MIN, RCD_MAX)
					}
					IF(RCD_TMIN>=0 && RCD_TMAX>=0) {
						VL="v"+Random(RCD_TMIN, RCD_TMAX)
						RCD_TMIN=RCD_TMIN+RCD_CRESC
						RCD_TMAX=RCD_TMAX+RCD_CRESC
						IF(RCD_TMIN<0) { RCD_TMIN=0 }
						IF(RCD_TMIN>127) { RCD_TMIN=127 }
						IF(RCD_TMAX<0) { RCD_TMAX=0 }
						IF(RCD_TMAX>127) { RCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+M
				}
			} ELSE {
				IF(M="#"||M="+"||M="-") {
					ML=ML+M
				} ELSE {
					IF(C>0) {
						ML=ML+VL+M
					} ELSE {
						FM=FM+VL+M
					}
				}
			}
		}
		VL="Sub{r-96v"+Random(RMD_VMIN, RMD_VMAX)+"}"
		BUF=BUF+"Sub"+CHR(123)+R+ML+CHR(125)+FIRST+"r"
		IF(DEBUG=1 || TEST=1) {
			Print(BUF)
		}
	}
	BUF
}

Function ROCT(Str MML) {
	Str BUF=""
	Str FIRST=""
	Str ML=""
	Str M
	Str R
	Str FM
	Int C=0
	Str VL

	MML=MML + ">" + MML + "<"

	IF(RCD_ENABLE=0) {
		BUF="'" + MML + "'"
	} ELSE {
		RCD_TMIN=RCD_VMIN
		RCD_TMAX=RCD_VMAX

		BUF=""
		R=""
		For(Int i=1; MID(MML, i, 1)!=""; i++) {
			M=MID(MML, i, 1)
			IF(DEBUG=1||TEST=1) {
				Print("Count "+ C + " Str="+M)
			}
			IF(M="a"||M="b"||M="c"||M="d"||M="e"||M="f"||M="g") {
				C++
			  	IF(C<2) {
					FIRST=ML
					ML=FM+M
					FM=""
					IF(RCD_TMIN>=0 && RCD_TMAX>=0) {
						VL="v"+Random(RCD_TMIN, RCD_TMAX)
						RCD_TMIN=RCD_TMIN+RCD_CRESC
						RCD_TMAX=RCD_TMAX+RCD_CRESC
						IF(RCD_TMIN<0) { RCD_TMIN=0 }
						IF(RCD_TMIN>127) { RCD_TMIN=127 }
						IF(RCD_TMAX<0) { RCD_TMAX=0 }
						IF(RCD_TMAX>127) { RCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+ML
				} ELSE {
					BUF=BUF+"Sub" + CHR(123)+R+ML+CHR(125)
					IF(RCD_MAX>0) {
						R=R+"r"+Random(RCD_MIN, RCD_MAX)
					}
					IF(RCD_TMIN>=0 && RCD_TMAX>=0) {
						VL="v"+Random(RCD_TMIN, RCD_TMAX)
						RCD_TMIN=RCD_TMIN+RCD_CRESC
						RCD_TMAX=RCD_TMAX+RCD_CRESC
						IF(RCD_TMIN<0) { RCD_TMIN=0 }
						IF(RCD_TMIN>127) { RCD_TMIN=127 }
						IF(RCD_TMAX<0) { RCD_TMAX=0 }
						IF(RCD_TMAX>127) { RCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+M
				}
			} ELSE {
				IF(M="#"||M="+"||M="-") {
					ML=ML+M
				} ELSE {
					IF(C>0) {
						ML=ML+VL+M
					} ELSE {
						FM=FM+VL+M
					}
				}
			}
		}
		BUF=BUF+"Sub"+CHR(123)+R+ML+CHR(125)+FIRST+"r"
		IF(DEBUG=1 || TEST=1) {
			Print(BUF)
		}
	}
	BUF
}

Function SRCDINIT(Int MIN, Int MAX, Int VMIN, Int VMAX, Int VDIF) {
	SRCD_MIN=MIN
	SRCD_MAX=MAX
	IF(VMIN<>0 && VMAX<>0) {
		SRCD_VMIN=VMIN
		SRCD_VMAX=VMAX
		SRCD_CRESC=VDIF
	} ELSE {
		SRCD_VMIN=-1
		SRCD_VMAX=-1
		SRCD_CRESC=0
	}
}

Function SRCDR(Int RMIN, Int RMAX) {
	SRCD_RMIN=RMIN
	SRCD_RMAX=RMAX
}

Function SRCDV(Int VMIN, Int VMAX) {
	SRCD_CRESC=0
	IF(VMAX=0) {
		Int TMPMIN=SRCD_VMIN
		Int TMPMAX=SRCD_VMAX
		TMPMIN=TMPMIN+VMIN
		TMPMAX=TMPMAX+VMIN
		IF(TMPMIN < 0) {
			TMPMIN=0
		}
		IF(TMPMIN > 127) {
			TMPMAX=127
		}
		IF(TMPMAX < 0) {
			TMPMAX=0
		}
		IF(TMPMAX > 127) {
			TMPMAX=127
		}
		SRCD_VMIN=TMPMIN
		SRCD_VMAX=TMPMAX
	} ELSE {
		SRCD_VMIN=VMIN
		SRCD_VMAX=VMAX
	}
}

Function SRCD(Str MML) {
	Str BUF=""
	Str FIRST=""
	Str ML=""
	Str M
	Str R
	Str FM
	Int C=0
	Str VL

	SRCD_TMIN=SRCD_VMIN
	SRCD_TMAX=SRCD_VMAX

	IF(SRCD_ENABLE=0) {
		BUF="'" + MML + "'"
	} ELSE {
		BUF=""
		R=""
		For(Int i=1; MID(MML, i, 1)!=""; i++) {
			M=MID(MML, i, 1)
			IF(DEBUG=1||TEST=1) {
				Print("Count "+ C + " Str="+M)
			}
			IF(M="a"||M="b"||M="c"||M="d"||M="e"||M="f"||M="g") {
				C++
			  	IF(C<2) {
					FIRST=ML
					ML=FM+M
					FM=""
					IF(SRCD_TMIN>=0 && SRCD_TMAX>=0) {
						VL="v"+Random(SRCD_TMIN, SRCD_TMAX)
						SRCD_TMIN=SRCD_TMIN+SRCD_CRESC
						SRCD_TMAX=SRCD_TMAX+SRCD_CRESC
						IF(SRCD_TMIN<0) { SRCD_TMIN=0 }
						IF(SRCD_TMIN>127) { SRCD_TMIN=127 }
						IF(SRCD_TMAX<0) { SRCD_TMAX=0 }
						IF(SRCD_TMAX>127) { SRCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+ML
				} ELSE {
					BUF=BUF+"Sub" + CHR(123)+R+ML+CHR(125)
					IF(SRCD_MAX>0) {
						R=R+"r"+Random(SRCD_MIN, SRCD_MAX)
					}
					IF(SRCD_TMIN>=0 && SRCD_TMAX>=0) {
						VL="v"+Random(SRCD_TMIN, SRCD_TMAX)
						SRCD_TMIN=SRCD_TMIN+SRCD_CRESC
						SRCD_TMAX=SRCD_TMAX+SRCD_CRESC
						IF(SRCD_TMIN<0) { SRCD_TMIN=0 }
						IF(SRCD_TMIN>127) { SRCD_TMIN=127 }
						IF(SRCD_TMAX<0) { SRCD_TMAX=0 }
						IF(SRCD_TMAX>127) { SRCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+M
				}
			} ELSE {
				IF(M="#"||M="+"||M="-") {
					ML=ML+M
				} ELSE {
					IF(C>0) {
						ML=ML+VL+M
					} ELSE {
						FM=FM+VL+M
					}
				}
			}
		}
		BUF=BUF+"Sub"+CHR(123)+R+ML+CHR(125)+FIRST+"r"
		IF(DEBUG=1 || TEST=1) {
			Print(BUF)
		}
	}
	BUF
}

Function SROCT(Str MML) {
	Str BUF=""
	Str FIRST=""
	Str ML=""
	Str M
	Str R
	Str FM
	Int C=0
	Str VL

	MML=MML + ">" + MML + "<"
	IF(SRCD_ENABLE=0) {
		BUF="'" + MML + "'"
	} ELSE {
		SRCD_TMIN=SRCD_VMIN
		SRCD_TMAX=SRCD_VMAX

		BUF=""
		R=""
		For(Int i=1; MID(MML, i, 1)!=""; i++) {
			M=MID(MML, i, 1)
			IF(DEBUG=1||TEST=1) {
				Print("Count "+ C + " Str="+M)
			}
			IF(M="a"||M="b"||M="c"||M="d"||M="e"||M="f"||M="g") {
				C++
			  	IF(C<2) {
					FIRST=ML
					ML=FM+M
					FM=""
					IF(SRCD_TMIN>=0 && SRCD_TMAX>=0) {
						VL="v"+Random(SRCD_TMIN, SRCD_TMAX)
						SRCD_TMIN=SRCD_TMIN+SRCD_CRESC
						SRCD_TMAX=SRCD_TMAX+SRCD_CRESC
						IF(SRCD_TMIN<0) { SRCD_TMIN=0 }
						IF(SRCD_TMIN>127) { SRCD_TMIN=127 }
						IF(SRCD_TMAX<0) { SRCD_TMAX=0 }
						IF(SRCD_TMAX>127) { SRCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+ML
				} ELSE {
					BUF=BUF+"Sub" + CHR(123)+R+ML+CHR(125)
					IF(SRCD_MAX>0) {
						R=R+"r"+Random(SRCD_MIN, SRCD_MAX)
					}
					IF(SRCD_TMIN>=0 && SRCD_TMAX>=0) {
						VL="v"+Random(SRCD_TMIN, SRCD_TMAX)
						SRCD_TMIN=SRCD_TMIN+SRCD_CRESC
						SRCD_TMAX=SRCD_TMAX+SRCD_CRESC
						IF(SRCD_TMIN<0) { SRCD_TMIN=0 }
						IF(SRCD_TMIN>127) { SRCD_TMIN=127 }
						IF(SRCD_TMAX<0) { SRCD_TMAX=0 }
						IF(SRCD_TMAX>127) { SRCD_TMAX=127 }
					} ELSE {
						VL=""
					}
					ML=VL+M
				}
			} ELSE {
				IF(M="#"||M="+"||M="-") {
					ML=ML+M
				} ELSE {
					IF(C>0) {
						ML=ML+VL+M
					} ELSE {
						FM=FM+VL+M
					}
				}
			}
		}
		BUF=BUF+"Sub"+CHR(123)+R+ML+CHR(125)+FIRST+"r"
		IF(DEBUG=1 || TEST=1) {
			Print(BUF)
		}
	}
	BUF
}

Function RDINIT(Int _RDVOL, Int _RANDV, Int _RANDR, Int _BDROTATE, Int _SDROTATE, Int _CYMROTATE) {
	RDVOL=_RDVOL
	RANDV=_RANDV
	RANDR=_RANDR
	BDROTATE=_BDROTATE
	SDROTATE=_SDROTATE
	CYMROTATE=_CYMROTATE
}

Function BassDrum(Int VO) {
	Int CC
	IF(BDROTATE=0) {
		CC=VO
	} ELSE {
		IF(BDINIT=0) {
			CC=BassDrum1
			BDINIT=1
		} ELSE {
				CC=BassDrum2
			BDINIT=0
		}
	}
	Result=CC
}

Function Snare(Int VO) {
	Int CC
	IF(SDROTATE=0) {
		CC=VO
	} ELSE {
		IF(SDINIT=0) {
			CC=Snare1
			SDINIT=1
		} ELSE {
				CC=Snare2
			SDINIT=0
		}
	}
	Result=CC
}

Function Cymbal(Int VO) {
	Int CC
	IF(CYMROTATE=0) {
		CC=VO
	} ELSE {
		IF(CYMINIT=0) {
			CC=CrashCymbal1
			CYMINIT=1
		} ELSE {
			CC=CrashCymbal2
			CYMINIT=0
		}
	}
	Result=CC
}

Function RDV(Int V) {
	RDVOL=V
}

Function RDCresc(Int V) {
	RDVOL=RDVOL+V
	IF(RDVOL>127) {
		RDVOL=127
	}
	IF(RDVOL<0) {
		RDVOL=0
	}
}

Function _RDV(Int V) {
	Int R=V
	R=R-(127-RDVOL)
	IF(R<0) {
		R=0
	}
	Result=R
}

Function RandV(Int MIN, Int MAX) {
	Str MML="v"
	IF(RANDV=0) {
		MML=MML+#STR(_RDV(MAX)
	} ELSE {
		MML=MML+Random(_RDV(MIN), _RDV(MAX))
	}
	MML
}

Str RMML
Function RandR(Int MIN, Int MAX) {
	Str MML
	IF(RANDR=1) {
		MML=MML+"r"
		RMML=RMML+"r"
		Int VALUE=Random(0,10)
		IF(VALUE<5) {
			MML=MML+"-"
		} ELSE {
			RMML=RMML+"-"
		}
		Str RAND=Random(MIN,MAX)
		MML=MML+RAND
		RMML=RMML+RAND
		MML
	}
}

Function SyncR() {
	Str MML=RMML
	RMML=""
	MML
}


#RDRUMSET1={
	// stick
	$k{Sub{RandR(800,1536) RandV(124,127) n(Sticks),16}r} //37

	// Bass Drum
	$B{Sub{RandR(800,1536) RandV(124,127) n(BassDrum(Kick2)),16}r}//35
	$b{Sub{RandR(800,1536) RandV(124,127) n(BassDrum(Kick1)),16}r}//36

	// Snare Drum
	$s{Sub{RandR(800,1536) RandV(120,127) n(Snare(Snare1)),16}r}//38
	$S{Sub{RandR(800,1536) RandV(120,127) n(Snare(Snare2)),16}r}//40

	// Toms
	$L{Sub{RandR(800,1536) RandV(123,127) n(LowTom2),16}r}//41
	$l{Sub{RandR(800,1536) RandV(123,127) n(LowTom1),16}r}//43
	$M{Sub{RandR(800,1536) RandV(123,127) n(MidTom2),16}r}//45
	$m{Sub{RandR(800,1536) RandV(123,127) n(MidTom1),16}r}//47
	$T{Sub{RandR(800,1536) RandV(123,127) n(HighTom2),16}r}//48
	$t{Sub{RandR(800,1536) RandV(123,127) n(HighTom1),16}r}//50

	// Hihat
	$p{Sub{RandR(800,1536) RandV(110,127) n(PedalHiHat),16}r}//44
	$h{Sub{RandR(800,1536) RandV( 65, 80) n(ClosedHiHat),16}r}//42
	$o{Sub{RandR(800,1536) RandV( 90,110) n(OpenHiHat),16}r}//46

	// Ride
	$r{Sub{RandR(800,1536) RandV(100,110) n(RideCymbal1),16}r}//51
	$R{Sub{RandR(800,1536) RandV(120,127) n(RideCymbal2),16}r}//59
	$P{Sub{RandR(800,1536) RandV(120,127) n(RideBell),16}r}//53

	// Cymbal
	$c{Sub{RandR(800,1536) RandV(120,127) n(Cymbal(CrashCymbal1)),16}r}//49
	$C{Sub{RandR(800,1536) RandV(120,127) n(Cymbal(CrashCymbal2)),16}r}//57

	// Splash
	$a{Sub{RandR(800,1536) RandV(120,127) n(SplashCymbal),16}r}//55
	$A{Sub{RandR(800,1536) RandV(120,127) n(ChineseCymbal),16}r}//52
}

/*
　■ rnd.h

	名目上、CCライセンスは、表示-継承としましたが、
	コード部がメインなのでGPLライセンスを適用して下さい

	Includeフォルダに入れて使います
	rnd.hとリネーム後、サクラやTWのIncludeフォルダに入れて下さい

	-- 拡張子"h"が、何らかのソフトにより登録されている場合、
	"rnd.h.mml"等とリネームされることがあります

 	この場合、フォルダオプションの"登録されている拡張子は表示しない"
	のチェックを一時外し、リネームしてみて下さい --

　■ 概要

	メロディーラインにごくわずかな休符を＋／－につけたり、音量（ベロシティー）を
　　範囲内でランダムに演奏させることで、リアルな演奏感をつけるものです。

　■ 基本雛型
	Include(rnd.h)
	TR(1)
	  RMDINIT(53,513,90,110)
	  l4 RMD(ceg)
	  RMDV(5)
	  l8 RMD(<b>fg)
	  RMDV(-5)
	  l8 RMD(ceg)
	TR(2)
	  RCDINIT(53,513)
	  l4 RCD(ceg)
	  l8 RCD(<b>fg)
	  RCDINIT(53,513,90,100)
	  l8 RCD(ceg)

　■ 関数 RMDINIT(rmin, rmax, vmin, vmax)
　　RMD機能の初期化を行います。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)

　■ 関数 RMD(mml)
　　MMLを生演奏に近くなるように演奏をします。
　　（ストトンは使えません）
　　サポートしているMMLリスト
		c- c c# d- d d# e- e e# f- f f# g- g g# a- a a# b- b b#
		r . ^
		1 2 3 4 5 6 7 8 9 0
	※数値は一応サポートしていますが、長さがうまくいかない場合、
　　　前に l8 等をつけて対応して下さい。

　　※和音はサポートしていません。RCD系の関数を使用して下さい。

　■ 関数 RMDR(rmin, rmax)
　　RMD機能の休符値のみ設定します。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)

　■ 関数 RMDV(vmin, vmax)
　　RMD機能の音量値のみ設定します。
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)

　■ 関数 RMDCresc(cresc)
　　ノートごとにcresc値の乱数音量値分を増量します。
　　マイナス値を設定すると、減量します。
	cresc : 音量増減分 (-127～127)

　■ 関数 RMDEnable(enabled)
　　RMD機能を有効にします。初期値は1です。
　　enabled : 1=有効、0=無効

　■ 関数 RCDINIT(rmin,rmax,vmin,vmax,vdif)
　　RCD機能の乱数値を設定する。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)
	vdif : 音量増加値（マイナスを設定すると減少値） (-127～127)

　■ 関数 RCD(mml)
	和音のMMLを生演奏に近くなるように演奏をします。
	すでに「'～'」で囲まれているのと同等です。

　　（ストトンは使えません）
　　サポートしているMMLリスト
		c- c c# d- d d# e- e e# f- f f# g- g g# a- a a# b- b b#
		r . ^
		1 2 3 4 5 6 7 8 9 0
	※数値は一応サポートしていますが、長さがうまくいかない場合、
　　　前に l8 等をつけて対応して下さい。

　　※単音はサポートしていません。RMD系の関数を使用して下さい。

　■ RCOT(mml)
　　単音のMMLを入れると自動的に１オクターブ上の音と一緒に和音で生演奏に
　　近くなるように演奏をします。

　　（ストトンは使えません）
　　サポートしているMMLリスト
		c- c c# d- d d# e- e e# f- f f# g- g g# a- a a# b- b b#
		r . ^
		1 2 3 4 5 6 7 8 9 0
	※数値は一応サポートしていますが、長さがうまくいかない場合、
　　　前に l8 等をつけて対応して下さい。

　■ 関数 RCDR(rmin, rmax)
　　RCD機能の休符値のみ設定します。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)

　■ 関数 RCDV(vmin, vmax)
　　RCD機能の音量値のみ設定します。
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)

　■ 関数 RCDEnable(enabled)
　　RCD機能を有効にします。初期値は1です。
　　enabled : 1=有効、0=無効

　■ 関数 SRCDINIT(rmin,rmax,vmin,vmax,vdif)
　　RCD機能（サブ）の乱数値を設定する。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)
	vdif : 音量増加値（マイナスを設定すると減少値） (-127～127)

　■ 関数 SRCD(mml)
	和音のMMLを生演奏に近くなるように演奏をします。
	すでに「'～'」で囲まれているのと同等です。

　　（ストトンは使えません）
　　サポートしているMMLリスト
		c- c c# d- d d# e- e e# f- f f# g- g g# a- a a# b- b b#
		r . ^
		1 2 3 4 5 6 7 8 9 0
	※数値は一応サポートしていますが、長さがうまくいかない場合、
　　　前に l8 等をつけて対応して下さい。

　　※単音はサポートしていません。RMD系の関数を使用して下さい。

　■ SRCOT(mml)
　　単音のMMLを入れると自動的に１オクターブ上の音と一緒に和音で生演奏に
　　近くなるように演奏をします。

　　（ストトンは使えません）
　　サポートしているMMLリスト
		c- c c# d- d d# e- e e# f- f f# g- g g# a- a a# b- b b#
		r . ^
		1 2 3 4 5 6 7 8 9 0
	※数値は一応サポートしていますが、長さがうまくいかない場合、
　　　前に l8 等をつけて対応して下さい。

　■ 関数 SRCDR(rmin, rmax)
　　RCD機能（サブ）の休符値のみ設定します。
	rmin : 休符の最小値 (1～System.TimeBaseの値)
	rmax : 休符の最大値 (1～System.TimeBaseの値)

　■ 関数 SRCDV(vmin, vmax)
　　RCD機能（サブ）の音量値のみ設定します。
	vmin : 音量の最小値 (0～127)
	vmax : 音量の最大値 (0～127)

　■ 関数 SRCDEnable(enabled)
　　RCD機能（サブ）を有効にします。初期値は1です。
　　enabled : 1=有効、0=無効

　■ RDINIT(rdvol,randv_flag,randr_flag,bdrotate,sdrotate,cymrotate)
	RD機能を初期化します。
	rdvol : ドラムの基本音量(0～127)
	randv_flag : ランダム音量フラグ 1=有効、0=無効
	randr_flag : ランダム休符フラグ 1=有効、0=無効
	bdrotate   : バスドラムロテートフラグ  1=有効、0=無効
　　sdrotate   : スネアドラムロテートフラグ  1=有効、0=無効
　　cymrotate  : シンバルロテートフラグ  1=有効、0=無効

　■ RDV(vol)
　　RD機能の基本音量を設定します(0～127)

　■ RDCresc(vol)
　　ノートごとに音量増をします。マイナス値を設定すると、減をします。 (0～127)

　■ RandV(Int MIN, Int MAX)
	ドラム定義マクロ内で使用する音量乱数範囲設定関数
	MIN : 最小音量 (0～127)
	MAX : 最大音量 (0～127)

　■ RandR(Int MIN, Int MAX)
	ドラム定義マクロ内で使用する休符乱数範囲設定関数
	MIN : 休符の最小値 (1～System.TimeBaseの値)
	MAX : 休符の最大値 (1～System.TimeBaseの値)

　■ SyncR()
	通常使用することはありませんが、ドラムの乱数休符を強制的にクリアする関数

　■ #RDRUMSET1
	既にRDVで定義してあるサンプルドラムセット
　　以下に対応します。
		k : Sticks
		B : Kick2 / b : Kick1
		s : Snare1 / S : Snare2
		L : LowTom2 / l : LowTom1
		M : MidTom2 / m : MidTom2
		T : HighTom2 / t : HighTom1
		p : PedalHihat / h : ClosedHihat / o : OPenHihat
		r : RideCymbal1 / R : RideCymbal2 / P : RideBell
		c : CrashCYmbal1 / C : CrashCymbal2
		a : SplashCymbal / A : ChineseCymbal

　■ ドラムセットの指定方法のサンプル
	$p{Sub{RandR(800,1536) RandV(110,127) n(PedalHiHat),16}r}
	$h{Sub{RandR(800,1536) RandV( 65, 80) n(ClosedHiHat),16}r}
	$o{Sub{RandR(800,1536) RandV( 90,110) n(OpenHiHat),16}r}

　　このマクロで指定することで、通常通りのドラムマクロで演奏感をつけることができます。
　　※ただし、ドラムノート音の長さに依存するものは、16の数字を適時変更するか、
	　通常の方法でマクロを指定して下さい。

　■ よりリアリティーある演奏にさせるためのコツ
　　より細かいシステムクォンタイズ(System.TimeBase)にすることをおすすめします。
	最低でも System.TimeBase=192に設定するようにしましょう。

　■ 現在わかってる問題
	MMLに数字を入れて長さを設定してもうまくいかないことがあります。
	その場合は、事前に、l8 等を設定して、長さを設定して下さい。

*/

#REV={
	IF(TWM2=1) {
		REV(50)
	}ELSE{
		REV(100)
	}
}

#R={
	RCDINIT(560,800)
	RMDINIT(320,800,90,110)
}

IF(DY=0) {
	#SP={
		RMDCresc(0)
		RMDV(33,36)
		RCDV(33,36)
		Sub {RMD(r96)}
	}

	#PP={
		RMDCresc(0)
		RMDV(33,48)
		RCDV(33,48)
		Sub {RMD(r96)}
	}

	#PPS={
		RMDCresc(0)
		RMDV(40,56)
		RCDV(40,56)
		Sub {RMD(r96)}
	}

	#P={
		RMDCresc(0)
		RMDV(48,64)
		RCDV(48,64)
		Sub {RMD(r96)}
	}

	#PS={
		RMDCresc(0)
		RMDV(56,72)
		RCDV(56,72)
		Sub {RMD(r96)}
	}

	#M={
		RMDCresc(0)
		RMDV(64,80)
		RCDV(64,80)
		Sub {RMD(r96)}
	}

	#MS={
		RMDCresc(0)
		RMDV(72,88)
		RCDV(72,88)
		Sub {RMD(r96)}
	}

	#F={
		RMDCresc(0)
		RMDV(80,96)
		RCDV(80,96)
		Sub {RMD(r96)}
	}

	#FS={
		RMDCresc(0)
		RMDV(88,104)
		RCDV(88,104)
		Sub {RMD(r96)}
	}

	#FF={
		RMDCresc(0)
		RMDV(96,112)
		RCDV(96,112)
		Sub {RMD(r96)}
	}

	#FFS={
		RMDCresc(0)
		RMDV(104,120)
		RCDV(104,120)
		Sub {RMD(r96)}
	}

	#SF={
		RMDCresc(0)
		RMDV(112,127)
		RCDV(112,127)
		Sub {RMD(r96)}
	}

	#SSF={
		RMDCresc(0)
		RMDV(127,127)
		RCDV(127,127)
		Sub {RMD(r96)}
	}

} ELSE {
	#SP={
		RMDCresc(0)
		RMDV(36,52)
		RCDV(36,52)
		Sub {RMD(r96)}
	}

	#PP={
		RMDCresc(0)
		RMDV(52,64)
		RCDV(52,64)
		Sub {RMD(r96)}
	}

	#PPS={
		RMDCresc(0)
		RMDV(56,72)
		RCDV(56,72)
		Sub {RMD(r96)}
	}

	#P={
		RMDCresc(0)
		RMDV(64,80)
		RCDV(64,80)
		Sub {RMD(r96)}
	}

	#PS={
		RMDCresc(0)
		RMDV(72,84)
		RCDV(72,84)
		Sub {RMD(r96)}
	}

	#M={
		RMDCresc(0)
		RMDV(80,92)
		RCDV(80,92)
		Sub {RMD(r96)}
	}

	#MS={
		RMDCresc(0)
		RMDV(84,96)
		RCDV(84,96)
		Sub {RMD(r96)}
	}

	#F={
		RMDCresc(0)
		RMDV(92,108)
		RCDV(92,108)
		Sub {RMD(r96)}
	}

	#FS={
		RMDCresc(0)
		RMDV(96,112)
		RCDV(96,112)
		Sub {RMD(r96)}
	}

	#FF={
		RMDCresc(0)
		RMDV(108,120)
		RCDV(108,120)
		Sub {RMD(r96)}
	}

	#FFS={
		RMDCresc(0)
		RMDV(112,124)
		RCDV(112,124)
		Sub {RMD(r96)}
	}

	#SF={
		RMDCresc(0)
		RMDV(120,127)
		RCDV(120,127)
		Sub {RMD(r96)}
	}

	#SSF={
		RMDCresc(0)
		RMDV(127,127)
		RCDV(127,127)
		Sub {RMD(r96)}
	}
}

Key(KEY)

TR(1)
	#REV

TR(2)
	#REV

TR(3)
	#REV

TR(4)
	#REV


曲名={"ノクターン2番変ホ長調作品9-2(打ち込み版)"}
作者={"ななこっち★ based by Fryderyk Franciszek Chopin"}

拍子 12,8

#PEDALON={Sub {r24y64,127}}
#PEDALOFF={Sub{r-48 RandR(50,64) y64,0}}
//#PEDALOFF={Sub{r-32 y64,0 y64,0}}
