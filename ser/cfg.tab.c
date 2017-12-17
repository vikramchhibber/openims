/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 83 "cfg.y" /* yacc.c:339  */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "route_struct.h"
#include "globals.h"
#include "route.h"
#include "dprint.h"
#include "sr_module.h"
#include "modparam.h"
#include "ip_addr.h"
#include "resolve.h"
#include "socket_info.h"
#include "name_alias.h"
#include "ut.h"
#include "dset.h"
#include "select.h"
#include "flags.h"

#include "config.h"
#ifdef CORE_TLS
#include "tls/tls_config.h"
#endif

#ifdef DEBUG_DMALLOC
#include <dmalloc.h>
#endif

/* hack to avoid alloca usage in the generated C file (needed for compiler
 with no built in alloca, like icc*/
#undef _ALLOCA_H

#define onsend_check(s) \
	do{\
		if (rt!=ONSEND_ROUTE) yyerror( s " allowed only in onsend_routes");\
	}while(0)


#ifdef USE_DNS_CACHE
	#define IF_DNS_CACHE(x) x
#else
	#define IF_DNS_CACHE(x) warn("dns cache support not compiled in")
#endif

#ifdef USE_DNS_FAILOVER
	#define IF_DNS_FAILOVER(x) x
#else
	#define IF_DNS_FAILOVER(x) warn("dns failover support not compiled in")
#endif

#ifdef USE_DST_BLACKLIST
	#define IF_DST_BLACKLIST(x) x
#else
	#define IF_DST_BLACKLIST(x) warn("dst blacklist support not compiled in")
#endif

#ifdef USE_STUN
	#define IF_STUN(x) x
#else 
	#define IF_STUN(x) warn("stun support not compiled in")
#endif


extern int yylex();
static void yyerror(char* s);
static char* tmp;
static int i_tmp;
static struct socket_id* lst_tmp;
static int rt;  /* Type of route block for find_export */
static str* str_tmp;
static str s_tmp;
static struct ip_addr* ip_tmp;
static struct avp_spec* s_attr;
static select_t sel;
static select_t* sel_ptr;
static struct action *mod_func_action;

static void warn(char* s);
static struct socket_id* mk_listen_id(char*, int, int);


#line 154 "cfg.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "cfg.tab.h".  */
#ifndef YY_YY_CFG_TAB_H_INCLUDED
# define YY_YY_CFG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FORWARD = 258,
    FORWARD_TCP = 259,
    FORWARD_TLS = 260,
    FORWARD_UDP = 261,
    SEND = 262,
    SEND_TCP = 263,
    DROP = 264,
    RETURN = 265,
    BREAK = 266,
    LOG_TOK = 267,
    ERROR = 268,
    ROUTE = 269,
    ROUTE_FAILURE = 270,
    ROUTE_ONREPLY = 271,
    ROUTE_BRANCH = 272,
    ROUTE_SEND = 273,
    EXEC = 274,
    SET_HOST = 275,
    SET_HOSTPORT = 276,
    PREFIX = 277,
    STRIP = 278,
    STRIP_TAIL = 279,
    APPEND_BRANCH = 280,
    SET_USER = 281,
    SET_USERPASS = 282,
    SET_PORT = 283,
    SET_URI = 284,
    REVERT_URI = 285,
    FORCE_RPORT = 286,
    FORCE_TCP_ALIAS = 287,
    IF = 288,
    ELSE = 289,
    SET_ADV_ADDRESS = 290,
    SET_ADV_PORT = 291,
    FORCE_SEND_SOCKET = 292,
    URIHOST = 293,
    URIPORT = 294,
    MAX_LEN = 295,
    SETFLAG = 296,
    RESETFLAG = 297,
    ISFLAGSET = 298,
    SETAVPFLAG = 299,
    RESETAVPFLAG = 300,
    ISAVPFLAGSET = 301,
    METHOD = 302,
    URI = 303,
    FROM_URI = 304,
    TO_URI = 305,
    SRCIP = 306,
    SRCPORT = 307,
    DSTIP = 308,
    DSTPORT = 309,
    TOIP = 310,
    TOPORT = 311,
    SNDIP = 312,
    SNDPORT = 313,
    SNDPROTO = 314,
    SNDAF = 315,
    PROTO = 316,
    AF = 317,
    MYSELF = 318,
    MSGLEN = 319,
    RETCODE = 320,
    UDP = 321,
    TCP = 322,
    TLS = 323,
    DEBUG_V = 324,
    FORK = 325,
    LOGSTDERROR = 326,
    LOGFACILITY = 327,
    LISTEN = 328,
    ALIAS = 329,
    DNS = 330,
    REV_DNS = 331,
    DNS_TRY_IPV6 = 332,
    DNS_RETR_TIME = 333,
    DNS_RETR_NO = 334,
    DNS_SERVERS_NO = 335,
    DNS_USE_SEARCH = 336,
    DNS_USE_CACHE = 337,
    DNS_USE_FAILOVER = 338,
    DNS_CACHE_FLAGS = 339,
    DNS_CACHE_NEG_TTL = 340,
    DNS_CACHE_MIN_TTL = 341,
    DNS_CACHE_MAX_TTL = 342,
    DNS_CACHE_MEM = 343,
    DNS_CACHE_GC_INT = 344,
    USE_DST_BLST = 345,
    DST_BLST_MEM = 346,
    DST_BLST_TTL = 347,
    DST_BLST_GC_INT = 348,
    PORT = 349,
    STAT = 350,
    CHILDREN = 351,
    CHECK_VIA = 352,
    SYN_BRANCH = 353,
    MEMLOG = 354,
    MEMDBG = 355,
    SIP_WARNING = 356,
    SERVER_SIGNATURE = 357,
    REPLY_TO_VIA = 358,
    LOADMODULE = 359,
    MODPARAM = 360,
    MAXBUFFER = 361,
    USER = 362,
    GROUP = 363,
    CHROOT = 364,
    WDIR = 365,
    MHOMED = 366,
    DISABLE_TCP = 367,
    TCP_ACCEPT_ALIASES = 368,
    TCP_CHILDREN = 369,
    TCP_CONNECT_TIMEOUT = 370,
    TCP_SEND_TIMEOUT = 371,
    TCP_CON_LIFETIME = 372,
    TCP_POLL_METHOD = 373,
    TCP_MAX_CONNECTIONS = 374,
    DISABLE_TLS = 375,
    ENABLE_TLS = 376,
    TLSLOG = 377,
    TLS_PORT_NO = 378,
    TLS_METHOD = 379,
    TLS_HANDSHAKE_TIMEOUT = 380,
    TLS_SEND_TIMEOUT = 381,
    SSLv23 = 382,
    SSLv2 = 383,
    SSLv3 = 384,
    TLSv1 = 385,
    TLS_VERIFY = 386,
    TLS_REQUIRE_CERTIFICATE = 387,
    TLS_CERTIFICATE = 388,
    TLS_PRIVATE_KEY = 389,
    TLS_CA_LIST = 390,
    ADVERTISED_ADDRESS = 391,
    ADVERTISED_PORT = 392,
    DISABLE_CORE = 393,
    OPEN_FD_LIMIT = 394,
    MCAST_LOOPBACK = 395,
    MCAST_TTL = 396,
    TOS = 397,
    KILL_TIMEOUT = 398,
    FLAGS_DECL = 399,
    AVPFLAGS_DECL = 400,
    ATTR_MARK = 401,
    SELECT_MARK = 402,
    ATTR_FROM = 403,
    ATTR_TO = 404,
    ATTR_FROMURI = 405,
    ATTR_TOURI = 406,
    ATTR_FROMUSER = 407,
    ATTR_TOUSER = 408,
    ATTR_FROMDOMAIN = 409,
    ATTR_TODOMAIN = 410,
    ATTR_GLOBAL = 411,
    ADDEQ = 412,
    STUN_REFRESH_INTERVAL = 413,
    STUN_ALLOW_STUN = 414,
    STUN_ALLOW_FP = 415,
    EQUAL = 416,
    EQUAL_T = 417,
    GT = 418,
    LT = 419,
    GTE = 420,
    LTE = 421,
    DIFF = 422,
    MATCH = 423,
    LOG_OR = 424,
    LOG_AND = 425,
    BIN_OR = 426,
    BIN_AND = 427,
    PLUS = 428,
    MINUS = 429,
    NOT = 430,
    NUMBER = 431,
    ID = 432,
    STRING = 433,
    IPV6ADDR = 434,
    COMMA = 435,
    SEMICOLON = 436,
    RPAREN = 437,
    LPAREN = 438,
    LBRACE = 439,
    RBRACE = 440,
    LBRACK = 441,
    RBRACK = 442,
    SLASH = 443,
    DOT = 444,
    CR = 445,
    COLON = 446,
    STAR = 447
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 171 "cfg.y" /* yacc.c:355  */

	long intval;
	unsigned long uval;
	char* strval;
	struct expr* expr;
	struct action* action;
	struct net* ipnet;
	struct ip_addr* ipaddr;
	struct socket_id* sockid;
	struct avp_spec* attr;
	select_t* select;

#line 400 "cfg.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CFG_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 415 "cfg.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  196
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2182

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  193
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  71
/* YYNRULES -- Number of rules.  */
#define YYNRULES  562
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1048

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   447

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   427,   427,   430,   431,   432,   435,   436,   437,   438,
     439,   439,   440,   440,   441,   441,   442,   442,   443,   443,
     444,   445,   448,   464,   473,   484,   485,   486,   487,   490,
     491,   494,   495,   496,   497,   498,   501,   502,   505,   506,
     508,   509,   512,   515,   521,   522,   526,   527,   530,   531,
     534,   540,   541,   542,   543,   544,   545,   546,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   597,
     598,   599,   600,   601,   602,   603,   604,   605,   606,   607,
     608,   609,   610,   611,   612,   613,   614,   615,   616,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   633,
     634,   641,   642,   649,   650,   657,   658,   665,   666,   673,
     674,   688,   702,   703,   710,   711,   718,   719,   726,   727,
     734,   735,   742,   743,   750,   757,   764,   771,   778,   785,
     786,   793,   794,   801,   802,   809,   810,   817,   818,   825,
     826,   833,   834,   835,   836,   837,   838,   846,   847,   851,
     852,   856,   857,   867,   868,   869,   870,   871,   872,   879,
     880,   887,   888,   889,   890,   891,   892,   893,   894,   895,
     896,   897,   898,   901,   907,   908,   913,   918,   921,   922,
     925,   954,   974,   975,   979,   989,   990,   994,   995,  1007,
    1010,  1013,  1025,  1028,  1031,  1043,  1046,  1049,  1061,  1063,
    1066,  1078,  1100,  1101,  1102,  1103,  1104,  1107,  1108,  1110,
    1111,  1112,  1113,  1114,  1117,  1118,  1121,  1122,  1125,  1126,
    1127,  1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,
    1140,  1141,  1142,  1143,  1145,  1146,  1147,  1148,  1150,  1151,
    1152,  1153,  1155,  1159,  1163,  1164,  1166,  1170,  1174,  1175,
    1177,  1178,  1179,  1181,  1183,  1187,  1191,  1192,  1194,  1195,
    1196,  1197,  1199,  1202,  1206,  1207,  1209,  1210,  1211,  1212,
    1213,  1215,  1216,  1217,  1218,  1220,  1221,  1233,  1234,  1236,
    1237,  1238,  1239,  1251,  1252,  1253,  1254,  1255,  1259,  1272,
    1276,  1280,  1281,  1282,  1286,  1299,  1303,  1307,  1308,  1310,
    1311,  1312,  1313,  1317,  1321,  1322,  1323,  1324,  1326,  1327,
    1328,  1329,  1330,  1331,  1333,  1334,  1335,  1336,  1339,  1340,
    1351,  1352,  1355,  1356,  1360,  1361,  1374,  1378,  1403,  1404,
    1405,  1406,  1409,  1410,  1413,  1414,  1415,  1418,  1419,  1420,
    1421,  1422,  1425,  1426,  1435,  1444,  1456,  1471,  1472,  1475,
    1475,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1496,  1499,  1500,  1503,  1510,  1513,  1520,  1526,  1527,  1530,
    1531,  1534,  1535,  1536,  1539,  1540,  1570,  1573,  1574,  1575,
    1576,  1577,  1578,  1581,  1582,  1583,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,
    1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,
    1627,  1635,  1643,  1651,  1659,  1667,  1675,  1683,  1691,  1692,
    1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,  1701,  1702,
    1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,  1712,
    1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,  1722,
    1728,  1734,  1735,  1740,  1746,  1747,  1752,  1758,  1759,  1764,
    1765,  1766,  1767,  1768,  1776,  1777,  1778,  1779,  1780,  1781,
    1782,  1783,  1784,  1785,  1786,  1787,  1788,  1789,  1790,  1791,
    1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,
    1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1817,
    1818,  1819,  1820,  1827,  1834,  1841,  1842,  1852,  1853,  1854,
    1869,  1870,  1871,  1872,  1873,  1874,  1874,  1888,  1890,  1891,
    1892,  1895,  1904
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FORWARD", "FORWARD_TCP", "FORWARD_TLS",
  "FORWARD_UDP", "SEND", "SEND_TCP", "DROP", "RETURN", "BREAK", "LOG_TOK",
  "ERROR", "ROUTE", "ROUTE_FAILURE", "ROUTE_ONREPLY", "ROUTE_BRANCH",
  "ROUTE_SEND", "EXEC", "SET_HOST", "SET_HOSTPORT", "PREFIX", "STRIP",
  "STRIP_TAIL", "APPEND_BRANCH", "SET_USER", "SET_USERPASS", "SET_PORT",
  "SET_URI", "REVERT_URI", "FORCE_RPORT", "FORCE_TCP_ALIAS", "IF", "ELSE",
  "SET_ADV_ADDRESS", "SET_ADV_PORT", "FORCE_SEND_SOCKET", "URIHOST",
  "URIPORT", "MAX_LEN", "SETFLAG", "RESETFLAG", "ISFLAGSET", "SETAVPFLAG",
  "RESETAVPFLAG", "ISAVPFLAGSET", "METHOD", "URI", "FROM_URI", "TO_URI",
  "SRCIP", "SRCPORT", "DSTIP", "DSTPORT", "TOIP", "TOPORT", "SNDIP",
  "SNDPORT", "SNDPROTO", "SNDAF", "PROTO", "AF", "MYSELF", "MSGLEN",
  "RETCODE", "UDP", "TCP", "TLS", "DEBUG_V", "FORK", "LOGSTDERROR",
  "LOGFACILITY", "LISTEN", "ALIAS", "DNS", "REV_DNS", "DNS_TRY_IPV6",
  "DNS_RETR_TIME", "DNS_RETR_NO", "DNS_SERVERS_NO", "DNS_USE_SEARCH",
  "DNS_USE_CACHE", "DNS_USE_FAILOVER", "DNS_CACHE_FLAGS",
  "DNS_CACHE_NEG_TTL", "DNS_CACHE_MIN_TTL", "DNS_CACHE_MAX_TTL",
  "DNS_CACHE_MEM", "DNS_CACHE_GC_INT", "USE_DST_BLST", "DST_BLST_MEM",
  "DST_BLST_TTL", "DST_BLST_GC_INT", "PORT", "STAT", "CHILDREN",
  "CHECK_VIA", "SYN_BRANCH", "MEMLOG", "MEMDBG", "SIP_WARNING",
  "SERVER_SIGNATURE", "REPLY_TO_VIA", "LOADMODULE", "MODPARAM",
  "MAXBUFFER", "USER", "GROUP", "CHROOT", "WDIR", "MHOMED", "DISABLE_TCP",
  "TCP_ACCEPT_ALIASES", "TCP_CHILDREN", "TCP_CONNECT_TIMEOUT",
  "TCP_SEND_TIMEOUT", "TCP_CON_LIFETIME", "TCP_POLL_METHOD",
  "TCP_MAX_CONNECTIONS", "DISABLE_TLS", "ENABLE_TLS", "TLSLOG",
  "TLS_PORT_NO", "TLS_METHOD", "TLS_HANDSHAKE_TIMEOUT", "TLS_SEND_TIMEOUT",
  "SSLv23", "SSLv2", "SSLv3", "TLSv1", "TLS_VERIFY",
  "TLS_REQUIRE_CERTIFICATE", "TLS_CERTIFICATE", "TLS_PRIVATE_KEY",
  "TLS_CA_LIST", "ADVERTISED_ADDRESS", "ADVERTISED_PORT", "DISABLE_CORE",
  "OPEN_FD_LIMIT", "MCAST_LOOPBACK", "MCAST_TTL", "TOS", "KILL_TIMEOUT",
  "FLAGS_DECL", "AVPFLAGS_DECL", "ATTR_MARK", "SELECT_MARK", "ATTR_FROM",
  "ATTR_TO", "ATTR_FROMURI", "ATTR_TOURI", "ATTR_FROMUSER", "ATTR_TOUSER",
  "ATTR_FROMDOMAIN", "ATTR_TODOMAIN", "ATTR_GLOBAL", "ADDEQ",
  "STUN_REFRESH_INTERVAL", "STUN_ALLOW_STUN", "STUN_ALLOW_FP", "EQUAL",
  "EQUAL_T", "GT", "LT", "GTE", "LTE", "DIFF", "MATCH", "LOG_OR",
  "LOG_AND", "BIN_OR", "BIN_AND", "PLUS", "MINUS", "NOT", "NUMBER", "ID",
  "STRING", "IPV6ADDR", "COMMA", "SEMICOLON", "RPAREN", "LPAREN", "LBRACE",
  "RBRACE", "LBRACK", "RBRACK", "SLASH", "DOT", "CR", "COLON", "STAR",
  "$accept", "cfg", "statements", "statement", "$@1", "$@2", "$@3", "$@4",
  "$@5", "listen_id", "proto", "port", "phostport", "id_lst", "flags_decl",
  "flag_list", "flag_spec", "flag_name", "avpflags_decl", "avpflag_list",
  "avpflag_spec", "assign_stm", "module_stm", "ip", "ipv4", "ipv6addr",
  "ipv6", "route_name", "route_stm", "failure_route_stm",
  "onreply_route_stm", "branch_route_stm", "send_route_stm", "exp",
  "equalop", "intop", "binop", "strop", "uri_type", "exp_elem", "ipnet",
  "host_sep", "host", "fcmd", "exp_stm", "stm", "actions", "action",
  "if_cmd", "select_param", "select_params", "select_id", "$@6",
  "attr_class_spec", "attr_name_spec", "attr_spec", "attr_mark", "attr_id",
  "attr_id_num_idx", "attr_id_no_idx", "attr_id_ass", "attr_id_val",
  "attr_id_any", "attr_id_any_str", "assign_op", "assign_action",
  "avpflag_oper", "cmd", "$@7", "func_params", "func_param", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447
};
# endif

#define YYPACT_NINF -603

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-603)))

#define YYTABLE_NINF -401

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     618,   -80,   -60,   -41,   -25,   228,   239,   259,   266,   282,
      52,   365,   367,   515,   524,   663,   976,   977,   978,   979,
    1006,  1026,  1027,  1028,  1049,  1074,  1075,   291,   298,   309,
     315,   353,   363,   377,   516,   605,   610,    40,     7,   611,
     614,   636,   679,   700,   713,   714,   737,   738,   811,   812,
     815,   825,   862,   877,   878,   884,   887,   891,   894,   900,
     901,   904,   908,   909,   910,   916,   918,   920,   922,   923,
     924,   927,   942,   182,   193,   943,   944,   945,  -603,  -603,
     106,   471,  -603,    75,   116,   152,   439,   500,  -603,  -603,
    -603,  -603,  -603,    34,   477,   478,    44,    28,    47,   480,
     482,  -603,   465,  -603,   483,  -603,   487,  -603,   492,  -603,
     624,  -603,   777,  -603,   809,  -603,   881,  -603,   890,  -603,
     946,  -603,   950,  -603,   951,  -603,   952,  -603,   980,  -603,
     985,  -603,   992,  -603,   996,   489,   889,   490,   491,   506,
     507,   508,   510,   597,   598,  -603,  -603,  -603,  1012,   645,
     196,   210,   296,   318,   646,   668,   669,   670,   744,   745,
     746,   324,   747,   763,   767,   768,   769,  1159,   778,   779,
     780,   781,   185,   444,   445,   215,   782,   783,   784,   785,
     786,   787,   788,  -603,  -603,  -603,  -603,   873,   964,  -603,
    -603,  -603,  1031,   789,   790,   791,  -603,   -80,  -603,     5,
    -603,    56,  -603,    72,  -603,    86,  -603,    92,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,   968,  -603,  -603,  -603,   984,  -603,  1042,  1061,  1383,
    -603,  -603,  -603,  -603,  -603,  -112,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  1076,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,   352,  1081,   352,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  2001,   -30,  -603,  2001,   -30,
    -603,  2001,   -30,  -603,  2001,   -30,  -603,  2001,   -30,  1082,
    1079,    43,   253,  -603,  -603,   128,   988,  1089,  -603,  -603,
    -603,    13,    32,    54,    60,    84,    90,    -6,   -19,  -603,
      97,    98,   104,  1085,   108,   110,   125,   131,   134,  1087,
     137,   143,   148,   149,  1122,  1126,  1127,  1690,   151,   154,
     158,   162,   164,   166,  -603,  -603,  -603,  -603,  -603,  -603,
      22,   806,  -603,  -603,  1673,  -603,  -603,  1110,  1130,   168,
    -603,  -603,  -603,  -603,  1144,   859,  1152,   905,  1156,   989,
    1158,  1088,  1164,  1165,  -603,  -603,  -603,  -603,  -603,  1161,
    -603,  -603,  1179,  -603,    69,  -603,    85,  -603,   123,  -603,
     200,  -603,   229,  -603,   235,  -603,  -603,  -102,  -603,  -603,
    -603,   334,  -603,   446,  -603,   288,  1182,  -603,   447,  -603,
     448,  -603,   449,  -603,   792,  -603,   793,  -128,  -603,   450,
    -603,   459,  -603,   460,  -603,   461,  1180,  1181,   179,   355,
    -603,  -603,  -603,   875,   639,   892,   981,   896,   987,   906,
    1157,  1163,  1170,  1205,  1211,   822,  1219,  1386,  -603,  1690,
    -603,  1690,  2001,  1767,   974,  -603,  -603,  -603,  -603,   242,
    1673,  1759,  1896,  1200,  1907,  -603,  -603,  -603,   240,  -603,
     794,  -603,    74,  -603,    -5,  -603,   204,  -603,   260,  1183,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  1190,  -603,  1194,  -603,  1949,
    -603,  -603,   -99,  1204,  -603,  1209,  -603,  1213,  -603,  1216,
    -603,  1217,  1240,  -120,  -100,  1236,   -72,   -63,   -26,   867,
    1237,   -22,   203,   275,   898,  1253,   287,   295,   333,  1166,
    1257,   357,   475,   585,  1176,  1258,   616,   673,  1391,  1287,
     674,   677,  1392,  1288,  -603,  1289,  1273,  1290,  1306,  1312,
    1311,  1320,  1333,  1334,  1335,  1336,  1352,  1356,  1357,  1358,
    1373,  1374,  1376,   824,  -603,  1381,  1385,  1388,  1394,  1395,
    1397,  1402,  1417,  -603,  -603,  1419,  1420,  -603,  -603,  -603,
    -603,  -603,  -603,   256,  -603,   249,   342,  -603,  -603,  -603,
    -603,  -603,  -603,   497,  -603,   795,   423,  -603,   498,  -603,
     983,   440,  -603,   499,  -603,  1378,   443,  -603,   502,  -603,
      16,  -603,   503,  -603,    36,  -603,   504,  -603,  1341,  -603,
     496,  -603,   505,  1317,  -603,  -118,  1172,  1690,  1690,  2001,
    1534,  -603,  -603,  1520,   276,   -81,  1432,  -603,  -603,  1446,
    1448,  1456,   -78,  1452,  1455,  1458,  1460,  1462,  1471,  1473,
    1476,  1477,  1479,  1481,  1482,   264,  1459,  1478,  -603,  -603,
    1690,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  1673,  -603,
    1487,  2001,  2001,  2001,  2001,  2001,  1480,  -603,  1486,  1488,
    -603,   -12,  -603,  1496,  -603,  1497,  -603,  1502,  -603,  -603,
      19,  -603,  1503,  -603,  1504,  -603,  1507,  -603,  -603,    24,
    -603,  1508,  -603,  1510,  -603,  1511,  -603,  -603,   345,  -603,
    1512,  -603,  1513,  -603,  1514,  -603,  -603,  1515,  -603,  1516,
    -603,  1529,  -603,  -603,  1530,  -603,  1531,  -603,  1532,  -603,
    -603,  -603,  1546,  -603,  -603,  1550,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  1551,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  1673,  -603,  -603,  -603,  -603,
    1568,  -603,  -603,  -603,  -112,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -112,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -112,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -112,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  1571,  -603,  1569,
    -603,  -603,  1589,  -603,  1218,  1873,  -603,  -603,  -603,  -112,
    -603,  -603,  -603,  -603,  -603,  -116,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,   187,  -603,  -603,  -603,   -48,
    -603,   352,  1271,  1401,  1454,  1500,  1584,  1586,  -603,  -603,
    1578,  1581,  1582,  1585,  1600,  1601,  1602,  1603,  1619,  1623,
    1624,  1625,  1633,  1635,  1637,  1648,  1649,  1650,  1651,  1652,
    1656,  1657,  1659,  1660,  1661,  1662,  1664,  1666,  1674,  1580,
      89,   484,  1317,  -603,  -603,  1672,  -603,   264,  -603,  -603,
    1678,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  1592,  -603,   968,  -603,
    1675,  1676,  -603,  -603,  -603,  -603,  -603,  -603
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    21,
       0,     0,     4,     0,     0,     0,     0,     0,     7,     8,
       6,     9,   202,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    66,     0,    68,     0,    70,     0,    72,
       0,    74,     0,    76,     0,    78,     0,    80,     0,    84,
       0,    82,     0,    86,     0,    88,     0,    90,     0,    92,
       0,    94,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   203,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,    45,    44,    38,    40,    42,    47,
      50,    46,    48,     0,     0,     0,     1,     5,     3,     0,
      11,     0,    13,     0,    15,     0,    17,     0,    19,    52,
      51,    54,    53,    56,    55,    58,    57,   177,    25,    26,
      27,     0,   354,    23,   211,     0,    28,    31,     0,    36,
     176,    22,   208,   212,   209,    24,   179,   178,    60,    59,
      62,    61,    63,    65,    67,    69,    71,    73,    75,    77,
      79,    83,    81,    85,    87,    89,    91,    93,    95,   101,
      97,    98,   103,   102,   105,   104,   107,   106,   109,   108,
     111,   110,   113,   112,   173,   172,   175,   174,     0,   100,
      99,   116,   115,   114,   119,   118,   117,   122,   121,   120,
     125,   124,   123,   127,   126,   129,   128,   131,   130,   133,
     132,   135,   134,   137,   136,   139,   138,   142,   140,   141,
     144,   143,   146,   145,   148,   147,   150,   149,   152,   151,
     157,   153,   154,   155,   156,   169,   168,   171,   170,   159,
     158,   161,   160,   163,   162,   165,   164,   167,   166,   181,
     180,   183,   182,   185,   184,   187,   186,   189,   188,   191,
     190,   193,   192,   195,   194,     0,     0,     0,   197,   196,
     199,   198,   201,   200,   219,     0,     0,   222,     0,     0,
     225,     0,     0,   228,     0,     0,   231,     0,     0,     0,
       0,     0,     0,    37,   353,     0,     0,     0,    41,    43,
      49,     0,     0,     0,     0,     0,     0,   480,   481,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   522,
       0,     0,     0,     0,   539,   541,   544,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   393,   555,   370,
       0,     0,   365,   368,     0,   397,   398,     0,     0,     0,
     357,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   213,    35,    29,    30,    32,    33,
     356,   355,     0,   425,     0,   447,     0,   458,     0,   436,
       0,   466,     0,   474,     0,   479,   478,     0,   483,   482,
     487,     0,   501,     0,   504,     0,     0,   508,     0,   524,
       0,   511,     0,   517,     0,   514,     0,     0,   530,     0,
     533,     0,   527,     0,   536,     0,     0,     0,     0,     0,
     248,   249,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,     0,
     337,     0,     0,     0,     0,   236,   358,   336,   359,   344,
       0,   401,   403,   402,     0,   338,   360,   548,     0,   551,
       0,   554,     0,   491,     0,   494,     0,   497,     0,     0,
     371,   367,   366,   217,   364,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,     0,   391,   394,   406,     0,
     369,   499,     0,     0,   220,     0,   223,     0,   226,     0,
     229,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   476,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   540,     0,     0,   543,   256,   237,
     238,   247,   246,     0,   310,   246,     0,   267,   240,   241,
     242,   243,   239,     0,   316,   246,     0,   271,     0,   328,
     246,     0,   279,     0,   322,   246,     0,   275,     0,   287,
       0,   295,     0,   283,     0,   291,     0,   335,     0,   300,
       0,   304,     0,     0,   234,     0,     0,     0,     0,     0,
     372,   362,   263,   246,     0,     0,   394,   244,   245,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   557,     0,     0,   408,   407,
       0,   409,   411,   401,   403,   402,   410,   405,     0,   404,
       0,     0,     0,     0,     0,     0,     0,    34,     0,     0,
     426,     0,   424,     0,   417,     0,   418,     0,   416,   448,
       0,   446,     0,   439,     0,   440,     0,   438,   459,     0,
     457,     0,   450,     0,   451,     0,   449,   437,     0,   435,
       0,   428,     0,   429,     0,   427,   467,     0,   461,     0,
     462,     0,   460,   475,     0,   469,     0,   470,     0,   468,
     477,   488,     0,   485,   502,     0,   505,   503,   506,   509,
     507,   525,   523,   512,   510,   518,   516,   515,   513,     0,
     520,   531,   529,   534,   532,   528,   526,   537,   535,   545,
     542,   255,   254,   251,   253,     0,   399,   400,   252,   308,
     350,   305,   309,   306,   307,   266,   264,   265,   314,   311,
     315,   312,   313,   270,   268,   269,   326,   323,   327,   324,
     325,   278,   276,   277,   320,   317,   321,   318,   319,   274,
     272,   273,   286,   284,   285,   294,   292,   293,   282,   280,
     281,   290,   288,   289,   334,   330,   331,   333,   332,   329,
     299,   298,   296,   297,   303,   301,   302,   374,   378,   380,
     235,   361,   233,   232,     0,     0,   261,   262,   257,   258,
     260,   259,   345,   347,   346,     0,   341,   342,   339,   340,
     343,   547,   546,   550,   549,   553,   552,   489,   490,   492,
     493,   495,   496,   561,   562,     0,   559,   392,   396,     0,
     394,     0,     0,     0,     0,     0,     0,     0,   206,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,   363,   373,     0,   560,     0,   556,   412,
       0,   218,   221,   224,   227,   230,   210,   422,   423,   420,
     421,   419,   444,   445,   442,   443,   441,   455,   456,   453,
     454,   452,   433,   434,   431,   432,   430,   464,   465,   463,
     472,   473,   471,   486,   500,   519,     0,   351,   349,   348,
       0,     0,   377,   395,   558,   498,   375,   376
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -603,  -603,  -603,  1780,  -603,  -603,  -603,  -603,  -603,  -173,
    -602,  1275,  1318,   -55,  -603,  1509,  -603,   -74,  -603,  1533,
    -603,  -603,  -603,  -463,  -603,  1644,  -603,   969,  -603,  -603,
    -603,  -603,  -603,  -509,  1132,  1331,  -603,  1301,  1173,  -603,
    -217,  -603,  -432,  -413,  -603,   957,  -356,  -421,  -401,   897,
    -603,  -530,  -603,  -603,  1155,  -521,  -404,  -350,  -392,  -386,
    -603,   960,  1309,  -603,  -603,  -378,  -603,  -603,  -603,  -603,
     914
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    80,    81,    82,    83,    84,    85,    86,    87,   227,
     228,   458,   229,   230,    88,   186,   187,   188,    89,   191,
     192,    90,    91,   231,   232,   233,   234,   444,   200,   202,
     204,   206,   208,   533,   672,   673,   721,   663,   534,   535,
     851,   386,   235,   430,   537,   710,   431,   432,   433,   908,
     909,   539,   703,   575,   576,   577,   434,   435,   847,   436,
     437,   544,   545,   750,   579,   438,   439,   440,   559,   945,
     946
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     190,   598,   340,   603,   536,   608,   364,   613,   147,   617,
     564,   621,   445,   540,   463,   447,   538,   882,   449,   716,
     704,   451,   705,   560,   564,   542,   564,   960,   564,   217,
     564,   543,   599,   465,   604,   209,   609,   888,   614,   546,
     618,   145,   622,   237,   455,   215,   478,   427,   236,   742,
     643,   707,   708,   101,   644,   467,   456,   367,   965,   475,
     995,   469,   384,   970,   910,   427,   528,   541,   427,   528,
     595,   948,   457,   370,   623,   727,   758,   385,   759,   747,
     624,    92,   218,   219,   220,   471,   600,   373,   883,   199,
    1037,   473,   889,   376,   218,   219,   220,   922,   480,   482,
     928,    93,   218,   219,   220,   484,   196,   596,   761,   487,
     762,   489,   711,   218,   219,   220,   536,   763,   536,   764,
      94,   707,   708,   601,   605,   540,   491,   540,   538,   460,
     538,   201,   493,   844,   999,   495,    95,   542,   498,   542,
     218,   219,   220,   543,   500,   543,   441,   442,   443,   502,
     504,   546,   547,   546,   765,   549,   766,   479,   770,   551,
     771,   606,   427,   553,   961,   555,   741,   557,   203,   581,
     476,   729,   184,   185,   383,   540,   706,   477,   748,   541,
     655,   541,   427,   183,   920,   923,   333,   744,   996,   365,
     148,   366,   929,   745,   189,   966,   464,   281,   912,   913,
     971,   610,   850,   561,   221,   222,   223,   224,   226,   459,
     210,   284,   850,   102,   225,   466,   339,   850,   146,   456,
     226,   216,   850,   221,   222,   223,   224,   950,   226,   743,
     615,   949,   749,   225,   854,   457,   619,   468,   611,   226,
     368,   723,   369,   470,   862,   221,   222,   597,   224,   870,
     221,   222,   223,   224,   878,   225,   371,   841,   372,   845,
     225,   221,   222,   602,   224,  1038,   226,   472,   224,   845,
     374,   225,   375,   474,   845,   225,   377,   917,   378,   845,
     481,   483,   919,   190,   845,   564,   845,   485,   845,   630,
     845,   488,   845,   490,   536,   536,   845,   287,   845,   221,
     222,   607,   224,   540,   540,  -352,   538,   538,   492,   225,
     845,   845,   849,   846,   494,   542,   542,   496,   845,   290,
     499,   543,   543,   846,   989,   307,   501,   536,   846,   546,
     546,   503,   505,   846,   548,   625,   540,   550,   846,   538,
     846,   552,   846,   852,   846,   554,   846,   556,   542,   558,
     846,   582,   846,   914,   543,   656,   658,   541,   541,   184,
     185,   657,   546,   334,   846,   846,   103,   997,   105,   998,
     184,   185,   846,   282,   283,   724,   221,   222,   612,   224,
     731,   184,   185,   772,   975,   773,   225,   285,   286,    96,
     541,   221,   222,   223,   224,   952,   953,   954,   955,   956,
      97,   225,   427,   528,   659,   221,   222,   616,   224,   660,
     661,   221,   222,   620,   224,   225,   221,   222,   223,   224,
      98,   225,   427,   528,   860,   221,   225,    99,   224,   221,
     222,   223,   224,   842,   843,   225,   733,   184,   185,   225,
     943,   868,   944,   100,   876,   335,   337,   628,   633,   635,
     637,   645,   135,   222,   918,   774,   205,   775,   859,   136,
     647,   649,   651,   867,   441,   442,   443,   779,   875,   780,
     137,    -2,   197,   288,   289,   781,   138,   782,   211,   213,
     730,   238,   732,   240,   734,   -10,   -12,   -14,   -16,   -18,
     259,   262,   264,   564,   711,   291,   292,   900,   855,   863,
     871,   308,   309,   879,   885,   891,   904,   266,   268,   270,
     626,   272,   627,   783,   139,   784,   107,   659,   207,   222,
     853,   976,   660,   661,   140,   109,   104,  1039,   106,   184,
     185,   564,   564,   564,   564,   564,   901,   788,   141,   789,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,   274,   276,
     222,   861,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,   222,   869,     1,
     222,   877,   336,   338,   629,   634,   636,   638,   646,    75,
      76,    77,   -10,   -12,   -14,   -16,   -18,   648,   650,   652,
     667,   242,   427,   427,   427,   427,   279,   293,   427,   427,
     427,   427,    78,   212,   214,   790,   239,   791,   241,   243,
    1040,    79,  1041,   244,   111,   260,   263,   265,   245,   295,
     297,   299,   902,   856,   864,   872,   108,   142,   880,   886,
     892,   905,   267,   269,   271,   110,   273,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,   301,   303,   305,   310,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   312,   792,   143,   793,   314,   316,
     318,   144,   149,   275,   277,   150,    75,    76,    77,   325,
     327,   329,   331,   341,   343,   345,   347,   349,   351,   353,
     358,   360,   362,   639,   641,   725,   797,   151,   798,    78,
     246,   659,   668,   669,   670,   671,   660,   562,    79,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   280,   294,   697,   112,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     152,   418,   419,   420,   296,   298,   300,   421,   422,   423,
     424,   425,   426,   799,   804,   800,   805,   806,   858,   807,
     562,   153,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   154,   155,   664,  1000,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   674,   418,   419,   420,   679,   156,   157,
     421,   422,   423,   424,   425,   426,   562,   684,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     302,   304,   306,   311,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   313,
     418,   419,   420,   315,   317,   319,   421,   422,   423,   424,
     425,   426,   427,   247,   326,   328,   330,   332,   342,   344,
     346,   348,   350,   352,   354,   359,   361,   363,   640,   642,
     726,   221,   158,   159,   224,   712,   160,   113,   115,   117,
     119,   225,   677,   428,   659,   248,   161,   429,   682,   660,
     562,   563,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   829,   427,   830,   121,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   162,   418,   419,   420,   123,   125,   127,
     421,   422,   423,   424,   425,   426,   428,   659,   163,   164,
     429,   384,   660,   661,   584,   165,   866,   767,   166,   768,
     129,   427,   167,   355,   659,   168,   385,   249,   659,   660,
     661,   169,   170,   660,   661,   171,   250,   261,   659,   172,
     173,   174,   384,   660,   661,   131,   133,   175,   776,   176,
     777,   177,   428,   178,   179,   180,   429,   385,   181,   562,
     586,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   182,   193,   194,   195,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   251,   418,   419,   420,   252,   253,   254,   421,
     422,   423,   424,   425,   426,   427,   659,   114,   116,   118,
     120,   660,   661,   659,   668,   669,   670,   671,   660,   659,
     668,   669,   670,   671,   660,   356,   255,   379,   687,   221,
     320,   256,   224,   224,   689,   461,   428,   122,   257,   225,
     429,   691,   258,   562,   588,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   124,   126,   128,
     278,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   693,   418,   419,   420,
     130,   357,   695,   421,   422,   423,   424,   425,   426,   562,
     699,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   381,   427,   132,   134,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   382,   418,   419,   420,   387,   389,   453,   421,
     422,   423,   424,   425,   426,   428,   454,   462,   486,   429,
     497,   578,   562,   590,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   321,   322,   323,   324,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   506,   418,   419,   420,   507,
     508,   580,   421,   422,   423,   424,   425,   426,   427,   659,
     668,   669,   670,   671,   660,   659,   668,   669,   670,   671,
     660,   583,   659,   668,   669,   670,   671,   660,   446,   585,
     384,   448,   894,   587,   450,   589,   785,   452,   786,   428,
     384,   591,   593,   429,   592,   385,   794,   911,   795,   594,
     632,  -398,   653,   654,   427,   385,   735,   659,   668,   669,
     670,   671,   660,   659,   668,   669,   670,   671,   660,   736,
     737,   659,   668,   669,   670,   671,   660,   701,   751,   510,
     511,   512,   895,   752,   896,   428,   897,   753,   898,   429,
     754,   755,   562,   993,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   756,   427,   760,   769,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   778,   418,   419,   420,   787,
     796,   874,   421,   422,   423,   424,   425,   426,   428,   218,
     219,   220,   429,   812,   631,   562,  1001,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   803,
     810,   811,   813,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   814,   418,
     419,   420,   815,   816,   907,   421,   422,   423,   424,   425,
     426,   562,   817,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   818,   819,   820,   821,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   822,   418,   419,   420,   823,   824,
     825,   421,   422,   423,   424,   425,   426,   427,   659,   668,
     669,   670,   671,   660,   221,   826,   827,   224,   828,   221,
     222,   223,   224,   831,   225,   384,   384,   832,   915,   225,
     833,   801,   808,   802,   809,   226,   834,   835,   428,   836,
     385,   385,   429,   916,   837,   562,  1002,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   838,
     427,   839,   840,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   925,   418,
     419,   420,  -239,   848,   926,   421,   422,   423,   424,   425,
     426,   428,   927,   857,   931,   429,   574,   932,   865,  1003,
     933,   662,   934,   873,   935,   665,   427,   675,   881,   680,
     884,   685,   887,   936,   890,   937,   893,   698,   938,   939,
     903,   940,   906,   941,   942,   948,   713,   951,   958,   957,
     959,   662,   962,   963,   921,   924,   719,   428,   964,   967,
     968,   429,   930,   969,   972,  1004,   973,   974,   977,   978,
     979,   980,   981,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   982,   983,   984,   985,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   986,   418,   419,   420,   987,   988,
     427,   421,   422,   423,   424,   425,   426,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   990,   991,   992,   708,
    1007,   428,  1006,  1008,  1009,   429,  1036,  1010,   995,  1005,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,  1011,  1012,  1013,  1014,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,  1015,   418,   419,   420,  1016,  1017,  1018,   421,   422,
     423,   424,   425,   426,   666,  1019,   676,  1020,   681,  1021,
     686,   565,   566,   567,   568,   569,   570,   571,   572,   573,
    1022,  1023,  1024,  1025,  1026,   714,   427,   528,  1027,  1028,
     715,  1029,  1030,  1031,  1032,   722,  1033,   678,  1034,   683,
     574,   688,   690,   692,   694,   696,  1035,   700,   702,  1043,
    1045,   198,  1046,  1047,   388,   529,   530,   428,   757,   380,
     728,   899,   994,   531,   532,   720,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   746,  1042,
     390,   947,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,     0,   418,   419,
     420,  1044,     0,   427,   421,   422,   423,   424,   425,   426,
    -397,  -399,  -399,  -399,  -399,  -399,  -399,  -399,     0,     0,
    -399,  -399,     0,     0,     0,     0,   707,   708,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,   429,     0,
       0,   709,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,     0,     0,     0,     0,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,     0,     0,   418,   419,   420,     0,     0,     0,
     421,   422,   423,   424,   425,   426,     0,     0,     0,     0,
       0,     0,     0,     0,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,     0,     0,     0,   427,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,   418,   419,   420,     0,
       0,     0,   421,   422,   423,   424,   425,   426,     0,     0,
     428,     0,     0,     0,   429,     0,     0,   709,  -400,  -400,
    -400,  -400,  -400,  -400,  -400,     0,     0,  -400,  -400,   659,
     668,   669,   670,   671,   660,   661,     0,     0,   717,   718,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   738,   428,   739,     0,     0,
       0,     0,   740,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,   429
};

static const yytype_int16 yycheck[] =
{
      74,   464,   175,   466,   417,   468,     1,   470,     1,   472,
     431,   474,   368,   417,     1,   371,   417,     1,   374,   540,
     529,   377,   531,     1,   445,   417,   447,    39,   449,     1,
     451,   417,   464,     1,   466,     1,   468,     1,   470,   417,
     472,     1,   474,    98,     1,     1,    65,   146,     1,   579,
     178,   169,   170,     1,   182,     1,   176,     1,    39,    65,
     176,     1,   174,    39,   182,   146,   147,   417,   146,   147,
       1,   187,   192,     1,   176,     1,   176,   189,   178,   178,
     182,   161,    66,    67,    68,     1,     1,     1,   690,    14,
       1,     1,   694,     1,    66,    67,    68,   178,     1,     1,
     178,   161,    66,    67,    68,     1,     0,    38,   180,     1,
     182,     1,   533,    66,    67,    68,   529,   180,   531,   182,
     161,   169,   170,    38,     1,   529,     1,   531,   529,     1,
     531,    15,     1,   663,   182,     1,   161,   529,     1,   531,
      66,    67,    68,   529,     1,   531,   176,   177,   178,     1,
       1,   529,     1,   531,   180,     1,   182,   176,   180,     1,
     182,    38,   146,     1,   176,     1,   579,     1,    16,     1,
     176,   176,   177,   178,   229,   579,   532,   183,   582,   529,
       1,   531,   146,     1,   714,   715,     1,   579,     1,   184,
     183,   186,   722,   579,     1,   176,   183,     1,   707,   708,
     176,     1,   665,   181,   176,   177,   178,   179,   192,   382,
     176,     1,   675,   161,   186,   183,     1,   680,   178,   176,
     192,   177,   685,   176,   177,   178,   179,   748,   192,   579,
       1,   740,   582,   186,   666,   192,     1,   183,    38,   192,
     184,     1,   186,   183,   676,   176,   177,   178,   179,   681,
     176,   177,   178,   179,   686,   186,   184,     1,   186,   663,
     186,   176,   177,   178,   179,   176,   192,   183,   179,   673,
     184,   186,   186,   183,   678,   186,   184,     1,   186,   683,
     183,   183,   714,   357,   688,   706,   690,   183,   692,     1,
     694,   183,   696,   183,   707,   708,   700,     1,   702,   176,
     177,   178,   179,   707,   708,   177,   707,   708,   183,   186,
     714,   715,    63,   663,   183,   707,   708,   183,   722,     1,
     183,   707,   708,   673,   845,     1,   183,   740,   678,   707,
     708,   183,   183,   683,   183,     1,   740,   183,   688,   740,
     690,   183,   692,     1,   694,   183,   696,   183,   740,   183,
     700,   183,   702,   709,   740,   176,     1,   707,   708,   177,
     178,   182,   740,   178,   714,   715,     1,   180,     1,   182,
     177,   178,   722,   177,   178,   548,   176,   177,   178,   179,
     176,   177,   178,   180,    39,   182,   186,   177,   178,   161,
     740,   176,   177,   178,   179,   751,   752,   753,   754,   755,
     161,   186,   146,   147,   162,   176,   177,   178,   179,   167,
     168,   176,   177,   178,   179,   186,   176,   177,   178,   179,
     161,   186,   146,   147,     1,   176,   186,   161,   179,   176,
     177,   178,   179,   177,   178,   186,   176,   177,   178,   186,
     176,     1,   178,   161,     1,     1,     1,     1,     1,     1,
       1,     1,   161,   177,   178,   180,    17,   182,   675,   161,
       1,     1,     1,   680,   176,   177,   178,   180,   685,   182,
     161,     0,     1,   177,   178,   180,   161,   182,     1,     1,
     554,     1,   556,     1,   558,    14,    15,    16,    17,    18,
       1,     1,     1,   914,   915,   177,   178,     1,     1,     1,
       1,   177,   178,     1,     1,     1,     1,     1,     1,     1,
     176,     1,   178,   180,   161,   182,     1,   162,    18,   177,
     178,   176,   167,   168,   161,     1,   161,   990,   161,   177,
     178,   952,   953,   954,   955,   956,    40,   180,   161,   182,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,     1,     1,
     177,   178,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   177,   178,     1,
     177,   178,   178,   178,   178,   178,   178,   178,   178,   158,
     159,   160,    14,    15,    16,    17,    18,   178,   178,   178,
       1,   176,   146,   146,   146,   146,     1,     1,   146,   146,
     146,   146,   181,   176,   176,   180,   176,   182,   176,   176,
     176,   190,   178,   176,     1,   176,   176,   176,   176,     1,
       1,     1,   176,   176,   176,   176,   161,   161,   176,   176,
     176,   176,   176,   176,   176,   161,   176,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,     1,     1,     1,     1,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,     1,   180,   161,   182,     1,     1,
       1,   161,   161,   176,   176,   161,   158,   159,   160,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,   180,   161,   182,   181,
     176,   162,   163,   164,   165,   166,   167,     1,   190,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,   176,   176,     1,   161,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
     161,    35,    36,    37,   176,   176,   176,    41,    42,    43,
      44,    45,    46,   180,   180,   182,   182,   180,    63,   182,
       1,   161,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,   161,   161,     1,   951,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     1,    35,    36,    37,     1,   161,   161,
      41,    42,    43,    44,    45,    46,     1,     1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     176,   176,   176,   176,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,   176,
      35,    36,    37,   176,   176,   176,    41,    42,    43,    44,
      45,    46,   146,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   161,   161,   179,     1,   161,     1,     1,     1,
       1,   186,     1,   177,   162,   176,   161,   181,     1,   167,
       1,   185,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,   180,   146,   182,     1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,   161,    35,    36,    37,     1,     1,     1,
      41,    42,    43,    44,    45,    46,   177,   162,   161,   161,
     181,   174,   167,   168,   185,   161,    63,   180,   161,   182,
       1,   146,   161,   180,   162,   161,   189,   176,   162,   167,
     168,   161,   161,   167,   168,   161,   176,   178,   162,   161,
     161,   161,   174,   167,   168,     1,     1,   161,   180,   161,
     182,   161,   177,   161,   161,   161,   181,   189,   161,     1,
     185,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,   161,   161,   161,   161,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,   176,    35,    36,    37,   176,   176,   176,    41,
      42,    43,    44,    45,    46,   146,   162,   161,   161,   161,
     161,   167,   168,   162,   163,   164,   165,   166,   167,   162,
     163,   164,   165,   166,   167,   191,   176,   189,     1,   176,
       1,   176,   179,   179,     1,   177,   177,   161,   176,   186,
     181,     1,   176,     1,   185,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,   161,   161,   161,
     178,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     1,    35,    36,    37,
     161,   180,     1,    41,    42,    43,    44,    45,    46,     1,
       1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,   191,   146,   161,   161,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,   191,    35,    36,    37,   180,   176,   176,    41,
      42,    43,    44,    45,    46,   177,   187,   178,   183,   181,
     183,   161,     1,   185,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,   127,   128,   129,   130,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,   183,    35,    36,    37,   183,
     183,   181,    41,    42,    43,    44,    45,    46,   146,   162,
     163,   164,   165,   166,   167,   162,   163,   164,   165,   166,
     167,   187,   162,   163,   164,   165,   166,   167,   369,   187,
     174,   372,     1,   187,   375,   187,   180,   378,   182,   177,
     174,   187,   191,   181,   189,   189,   180,   185,   182,   180,
     178,   161,   182,   182,   146,   189,   183,   162,   163,   164,
     165,   166,   167,   162,   163,   164,   165,   166,   167,   189,
     186,   162,   163,   164,   165,   166,   167,     1,   184,    48,
      49,    50,    51,   184,    53,   177,    55,   184,    57,   181,
     184,   184,     1,   185,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,   176,   146,   182,   182,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,   182,    35,    36,    37,   182,
     182,    63,    41,    42,    43,    44,    45,    46,   177,    66,
      67,    68,   181,   180,   485,     1,   185,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,   182,
     182,   182,   182,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,   182,    35,
      36,    37,   180,   182,   177,    41,    42,    43,    44,    45,
      46,     1,   182,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   182,   182,   182,   182,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,   182,    35,    36,    37,   182,   182,
     182,    41,    42,    43,    44,    45,    46,   146,   162,   163,
     164,   165,   166,   167,   176,   182,   182,   179,   182,   176,
     177,   178,   179,   182,   186,   174,   174,   182,    34,   186,
     182,   180,   180,   182,   182,   192,   182,   182,   177,   182,
     189,   189,   181,    63,   182,     1,   185,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,   182,
     146,   182,   182,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,   186,    35,
      36,    37,   176,   663,   176,    41,    42,    43,    44,    45,
      46,   177,   176,   673,   182,   181,   177,   182,   678,   185,
     182,   509,   182,   683,   182,   513,   146,   515,   688,   517,
     690,   519,   692,   182,   694,   182,   696,   525,   182,   182,
     700,   182,   702,   182,   182,   187,   534,   180,   182,   189,
     182,   539,   176,   176,   714,   715,   544,   177,   176,   176,
     176,   181,   722,   176,   176,   185,   176,   176,   176,   176,
     176,   176,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   176,   176,   176,   176,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,   178,    35,    36,    37,   178,   178,
     146,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,   188,   186,   189,   170,
     182,   177,   176,   182,   182,   181,   186,   182,   176,   185,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,   182,   182,   182,   182,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,   182,    35,    36,    37,   182,   182,   182,    41,    42,
      43,    44,    45,    46,   513,   182,   515,   182,   517,   182,
     519,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     182,   182,   182,   182,   182,   534,   146,   147,   182,   182,
     539,   182,   182,   182,   182,   544,   182,   516,   182,   518,
     177,   520,   521,   522,   523,   524,   182,   526,   527,   187,
     182,    81,   187,   187,   355,   175,   176,   177,   593,   225,
     552,   698,   915,   183,   184,   544,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,   579,   992,
     357,   736,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,   997,    -1,   146,    41,    42,    43,    44,    45,    46,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,    -1,
     171,   172,    -1,    -1,    -1,    -1,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   181,    -1,
      -1,   184,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    35,    36,    37,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,   146,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    -1,    -1,
     177,    -1,    -1,    -1,   181,    -1,    -1,   184,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,   171,   172,   162,
     163,   164,   165,   166,   167,   168,    -1,    -1,   171,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   177,   178,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,   181
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   158,   159,   160,   181,   190,
     194,   195,   196,   197,   198,   199,   200,   201,   207,   211,
     214,   215,   161,   161,   161,   161,   161,   161,   161,   161,
     161,     1,   161,     1,   161,     1,   161,     1,   161,     1,
     161,     1,   161,     1,   161,     1,   161,     1,   161,     1,
     161,     1,   161,     1,   161,     1,   161,     1,   161,     1,
     161,     1,   161,     1,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,     1,   178,     1,   183,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,     1,   177,   178,   208,   209,   210,     1,
     210,   212,   213,   161,   161,   161,     0,     1,   196,    14,
     221,    15,   222,    16,   223,    17,   224,    18,   225,     1,
     176,     1,   176,     1,   176,     1,   177,     1,    66,    67,
      68,   176,   177,   178,   179,   186,   192,   202,   203,   205,
     206,   216,   217,   218,   219,   235,     1,   206,     1,   176,
       1,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,     1,
     176,   178,     1,   176,     1,   176,     1,   176,     1,   176,
       1,   176,     1,   176,     1,   176,     1,   176,   178,     1,
     176,     1,   177,   178,     1,   177,   178,     1,   177,   178,
       1,   177,   178,     1,   176,     1,   176,     1,   176,     1,
     176,     1,   176,     1,   176,     1,   176,     1,   177,   178,
       1,   176,     1,   176,     1,   176,     1,   176,     1,   176,
       1,   127,   128,   129,   130,     1,   176,     1,   176,     1,
     176,     1,   176,     1,   178,     1,   178,     1,   178,     1,
     202,     1,   176,     1,   176,     1,   176,     1,   176,     1,
     176,     1,   176,     1,   176,   180,   191,   180,     1,   176,
       1,   176,     1,   176,     1,   184,   186,     1,   184,   186,
       1,   184,   186,     1,   184,   186,     1,   184,   186,   189,
     218,   191,   191,   206,   174,   189,   234,   180,   208,   176,
     212,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    35,    36,
      37,    41,    42,    43,    44,    45,    46,   146,   177,   181,
     236,   239,   240,   241,   249,   250,   252,   253,   258,   259,
     260,   176,   177,   178,   220,   239,   220,   239,   220,   239,
     220,   239,   220,   176,   187,     1,   176,   192,   204,   202,
       1,   177,   178,     1,   183,     1,   183,     1,   183,     1,
     183,     1,   183,     1,   183,    65,   176,   183,    65,   176,
       1,   183,     1,   183,     1,   183,   183,     1,   183,     1,
     183,     1,   183,     1,   183,     1,   183,   183,     1,   183,
       1,   183,     1,   183,     1,   183,   183,   183,   183,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,   147,   175,
     176,   183,   184,   226,   231,   232,   236,   237,   241,   244,
     249,   250,   251,   252,   254,   255,   258,     1,   183,     1,
     183,     1,   183,     1,   183,     1,   183,     1,   183,   261,
       1,   181,     1,   185,   240,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   177,   246,   247,   248,   161,   257,
     181,     1,   183,   187,   185,   187,   185,   187,   185,   187,
     185,   187,   189,   191,   180,     1,    38,   178,   216,   235,
       1,    38,   178,   216,   235,     1,    38,   178,   216,   235,
       1,    38,   178,   216,   235,     1,   178,   216,   235,     1,
     178,   216,   235,   176,   182,     1,   176,   178,     1,   178,
       1,   220,   178,     1,   178,     1,   178,     1,   178,     1,
     176,     1,   176,   178,   182,     1,   178,     1,   178,     1,
     178,     1,   178,   182,   182,     1,   176,   182,     1,   162,
     167,   168,   227,   230,     1,   227,   230,     1,   163,   164,
     165,   166,   227,   228,     1,   227,   230,     1,   228,     1,
     227,   230,     1,   228,     1,   227,   230,     1,   228,     1,
     228,     1,   228,     1,   228,     1,   228,     1,   227,     1,
     228,     1,   228,   245,   226,   226,   239,   169,   170,   184,
     238,   240,     1,   227,   230,   230,   248,   171,   172,   227,
     228,   229,   230,     1,   202,     1,   176,     1,   205,   176,
     210,   176,   210,   176,   210,   183,   189,   186,   176,   178,
     183,   236,   244,   250,   251,   252,   255,   178,   249,   250,
     256,   184,   184,   184,   184,   184,   176,   204,   176,   178,
     182,   180,   182,   180,   182,   180,   182,   180,   182,   182,
     180,   182,   180,   182,   180,   182,   180,   182,   182,   180,
     182,   180,   182,   180,   182,   180,   182,   182,   180,   182,
     180,   182,   180,   182,   180,   182,   182,   180,   182,   180,
     182,   180,   182,   182,   180,   182,   180,   182,   180,   182,
     182,   182,   180,   182,   182,   180,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   180,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,     1,   177,   178,   244,   249,   250,   251,   254,    63,
     216,   233,     1,   178,   235,     1,   176,   254,    63,   233,
       1,   178,   235,     1,   176,   254,    63,   233,     1,   178,
     235,     1,   176,   254,    63,   233,     1,   178,   235,     1,
     176,   254,     1,   203,   254,     1,   176,   254,     1,   203,
     254,     1,   176,   254,     1,    51,    53,    55,    57,   231,
       1,    40,   176,   254,     1,   176,   254,   177,   242,   243,
     182,   185,   226,   226,   239,    34,    63,     1,   178,   235,
     244,   254,   178,   244,   254,   186,   176,   176,   178,   244,
     254,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   176,   178,   262,   263,   247,   187,   226,
     248,   180,   239,   239,   239,   239,   239,   189,   182,   182,
      39,   176,   176,   176,   176,    39,   176,   176,   176,   176,
      39,   176,   176,   176,   176,    39,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   178,   178,   178,   248,
     188,   186,   189,   185,   238,   176,     1,   180,   182,   182,
     210,   185,   185,   185,   185,   185,   176,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   186,     1,   176,   216,
     176,   178,   242,   187,   263,   182,   187,   187
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   193,   194,   195,   195,   195,   196,   196,   196,   196,
     197,   196,   198,   196,   199,   196,   200,   196,   201,   196,
     196,   196,   202,   202,   202,   203,   203,   203,   203,   204,
     204,   205,   205,   205,   205,   205,   206,   206,   207,   207,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   215,   215,   215,   215,   216,   216,
     217,   218,   219,   219,   220,   220,   220,   221,   221,   221,
     222,   222,   222,   223,   223,   223,   224,   224,   224,   225,
     225,   225,   226,   226,   226,   226,   226,   227,   227,   228,
     228,   228,   228,   228,   229,   229,   230,   230,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     233,   233,   234,   234,   235,   235,   235,   236,   237,   237,
     237,   237,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   240,   241,   241,   242,   242,   242,   243,   243,   245,
     244,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     247,   248,   248,   249,   250,   251,   252,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   257,   258,   258,   258,
     258,   258,   258,   259,   259,   259,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   261,   260,   262,   262,   262,
     262,   263,   263
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       0,     2,     0,     2,     0,     2,     0,     2,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     5,     3,     1,     2,     2,     2,
       1,     3,     1,     3,     1,     1,     2,     2,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     2,     3,     2,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     8,     8,     2,     1,     1,
       7,     1,     1,     3,     1,     1,     1,     4,     7,     2,
       4,     7,     2,     4,     7,     2,     4,     7,     2,     4,
       7,     2,     3,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     2,     3,     3,
       3,     2,     3,     3,     3,     2,     3,     3,     3,     2,
       3,     3,     3,     2,     3,     3,     3,     2,     3,     3,
       3,     2,     3,     3,     3,     2,     3,     3,     3,     3,
       2,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     3,
       3,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     3,     1,     3,     2,     1,     2,     2,     1,     2,
       1,     2,     3,     5,     1,     4,     4,     3,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     2,     5,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     5,     1,     1,     1,     4,     4,     4,     6,
       6,     6,     6,     6,     4,     2,     4,     4,     4,     4,
       6,     6,     6,     6,     6,     4,     2,     4,     4,     4,
       4,     6,     6,     6,     6,     6,     4,     2,     4,     4,
       4,     4,     6,     6,     6,     6,     6,     4,     2,     4,
       4,     4,     4,     6,     6,     6,     2,     4,     4,     4,
       4,     6,     6,     6,     2,     4,     3,     4,     2,     2,
       1,     1,     2,     2,     1,     4,     6,     2,     4,     4,
       4,     2,     4,     4,     2,     4,     4,     2,     6,     2,
       6,     2,     4,     4,     2,     4,     4,     4,     2,     4,
       4,     2,     4,     4,     2,     4,     4,     2,     4,     6,
       4,     3,     1,     4,     2,     4,     4,     2,     4,     4,
       2,     4,     4,     2,     4,     4,     2,     4,     3,     1,
       3,     1,     4,     3,     1,     4,     4,     4,     2,     4,
       4,     2,     4,     4,     2,     0,     5,     0,     3,     1,
       2,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 430 "cfg.y" /* yacc.c:1646  */
    {}
#line 2496 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 431 "cfg.y" /* yacc.c:1646  */
    {}
#line 2502 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 432 "cfg.y" /* yacc.c:1646  */
    { yyerror(""); YYABORT;}
#line 2508 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 439 "cfg.y" /* yacc.c:1646  */
    {rt=REQUEST_ROUTE;}
#line 2514 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 440 "cfg.y" /* yacc.c:1646  */
    {rt=FAILURE_ROUTE;}
#line 2520 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 441 "cfg.y" /* yacc.c:1646  */
    {rt=ONREPLY_ROUTE;}
#line 2526 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 442 "cfg.y" /* yacc.c:1646  */
    {rt=BRANCH_ROUTE;}
#line 2532 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 443 "cfg.y" /* yacc.c:1646  */
    {rt=ONSEND_ROUTE;}
#line 2538 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 448 "cfg.y" /* yacc.c:1646  */
    {
		tmp=ip_addr2a((yyvsp[0].ipaddr));
		if (tmp==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: bad ip "
					"address.\n");
			(yyval.strval)=0;
		} else {
			(yyval.strval)=pkg_malloc(strlen(tmp)+1);
			if ((yyval.strval)==0) {
				LOG(L_CRIT, "ERROR: cfg. parser: out of "
						"memory.\n");
			} else {
				strncpy((yyval.strval), tmp, strlen(tmp)+1);
			}
		}
	}
#line 2559 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 464 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.strval)=pkg_malloc(strlen((yyvsp[0].strval))+1);
		if ((yyval.strval)==0) {
				LOG(L_CRIT, "ERROR: cfg. parser: out of "
						"memory.\n");
		} else {
				strncpy((yyval.strval), (yyvsp[0].strval), strlen((yyvsp[0].strval))+1);
		}
	}
#line 2573 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 473 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.strval)=pkg_malloc(strlen((yyvsp[0].strval))+1);
		if ((yyval.strval)==0) {
				LOG(L_CRIT, "ERROR: cfg. parser: out of "
						"memory.\n");
		} else {
				strncpy((yyval.strval), (yyvsp[0].strval), strlen((yyvsp[0].strval))+1);
		}
	}
#line 2587 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 484 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_UDP; }
#line 2593 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 485 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TCP; }
#line 2599 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 486 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TLS; }
#line 2605 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 487 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=0; }
#line 2611 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 490 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=(yyvsp[0].intval); }
#line 2617 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 491 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=0; }
#line 2623 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 494 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[0].strval), 0, 0); }
#line 2629 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 495 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[-2].strval), 0, (yyvsp[0].intval)); }
#line 2635 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 496 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[0].strval), (yyvsp[-2].intval), 0); }
#line 2641 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 497 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[-2].strval), (yyvsp[-4].intval), (yyvsp[0].intval));}
#line 2647 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 498 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=0; yyerror(" port number expected"); }
#line 2653 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 501 "cfg.y" /* yacc.c:1646  */
    {  (yyval.sockid)=(yyvsp[0].sockid) ; }
#line 2659 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 502 "cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=(yyvsp[-1].sockid); (yyval.sockid)->next=(yyvsp[0].sockid); }
#line 2665 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 506 "cfg.y" /* yacc.c:1646  */
    { yyerror("flag list expected\n"); }
#line 2671 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 512 "cfg.y" /* yacc.c:1646  */
    { if (register_flag((yyvsp[0].strval),-1)<0)
								yyerror("register flag failed");
						}
#line 2679 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 515 "cfg.y" /* yacc.c:1646  */
    {
						if (register_flag((yyvsp[-2].strval), (yyvsp[0].intval))<0)
								yyerror("register flag failed");
										}
#line 2688 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 521 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 2694 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 522 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 2700 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 527 "cfg.y" /* yacc.c:1646  */
    { yyerror("avpflag list expected\n"); }
#line 2706 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 534 "cfg.y" /* yacc.c:1646  */
    {
		if (register_avpflag((yyvsp[0].strval))==0)
			yyerror("cannot declare avpflag");
	}
#line 2715 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 540 "cfg.y" /* yacc.c:1646  */
    { debug=(yyvsp[0].intval); }
#line 2721 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 541 "cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 2727 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 542 "cfg.y" /* yacc.c:1646  */
    { dont_fork= ! (yyvsp[0].intval); }
#line 2733 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 543 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2739 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 544 "cfg.y" /* yacc.c:1646  */
    { if (!config_check) log_stderr=(yyvsp[0].intval); }
#line 2745 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 545 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2751 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 546 "cfg.y" /* yacc.c:1646  */
    {
		if ( (i_tmp=str2facility((yyvsp[0].strval)))==-1)
			yyerror("bad facility (see syslog(3) man page)");
		if (!config_check)
			log_facility=i_tmp;
	}
#line 2762 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 552 "cfg.y" /* yacc.c:1646  */
    { yyerror("ID expected"); }
#line 2768 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 553 "cfg.y" /* yacc.c:1646  */
    { received_dns|= ((yyvsp[0].intval))?DO_DNS:0; }
#line 2774 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 554 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2780 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 555 "cfg.y" /* yacc.c:1646  */
    { received_dns|= ((yyvsp[0].intval))?DO_REV_DNS:0; }
#line 2786 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 556 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2792 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 557 "cfg.y" /* yacc.c:1646  */
    { dns_try_ipv6=(yyvsp[0].intval); }
#line 2798 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 558 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2804 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 559 "cfg.y" /* yacc.c:1646  */
    { dns_retr_time=(yyvsp[0].intval); }
#line 2810 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 560 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 2816 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 561 "cfg.y" /* yacc.c:1646  */
    { dns_retr_no=(yyvsp[0].intval); }
#line 2822 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 562 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 2828 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 563 "cfg.y" /* yacc.c:1646  */
    { dns_servers_no=(yyvsp[0].intval); }
#line 2834 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 564 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 2840 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 565 "cfg.y" /* yacc.c:1646  */
    { dns_search_list=(yyvsp[0].intval); }
#line 2846 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 566 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2852 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 567 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(use_dns_cache=(yyvsp[0].intval)); }
#line 2858 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 568 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2864 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 569 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_FAILOVER(use_dns_failover=(yyvsp[0].intval));}
#line 2870 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 570 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2876 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 571 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_flags=(yyvsp[0].intval)); }
#line 2882 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 572 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2888 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 573 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_neg_cache_ttl=(yyvsp[0].intval)); }
#line 2894 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 574 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2900 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 575 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_cache_max_ttl=(yyvsp[0].intval)); }
#line 2906 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 576 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2912 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 577 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_cache_min_ttl=(yyvsp[0].intval)); }
#line 2918 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 578 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2924 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 579 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_cache_max_mem=(yyvsp[0].intval)); }
#line 2930 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 580 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2936 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 581 "cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_timer_interval=(yyvsp[0].intval)); }
#line 2942 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 582 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2948 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 583 "cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(use_dst_blacklist=(yyvsp[0].intval)); }
#line 2954 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 584 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2960 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 585 "cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(blst_max_mem=(yyvsp[0].intval)); }
#line 2966 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 586 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2972 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 587 "cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(blst_timeout=(yyvsp[0].intval)); }
#line 2978 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 588 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2984 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 589 "cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(blst_timer_interval=(yyvsp[0].intval));}
#line 2990 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 590 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 2996 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 591 "cfg.y" /* yacc.c:1646  */
    { port_no=(yyvsp[0].intval); }
#line 3002 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 592 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef STATS
				stat_file=(yyvsp[0].strval);
		#endif
	}
#line 3012 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 597 "cfg.y" /* yacc.c:1646  */
    { maxbuffer=(yyvsp[0].intval); }
#line 3018 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 598 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3024 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 599 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3030 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 600 "cfg.y" /* yacc.c:1646  */
    { children_no=(yyvsp[0].intval); }
#line 3036 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 601 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3042 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 602 "cfg.y" /* yacc.c:1646  */
    { check_via=(yyvsp[0].intval); }
#line 3048 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 603 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3054 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 604 "cfg.y" /* yacc.c:1646  */
    { syn_branch=(yyvsp[0].intval); }
#line 3060 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 605 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3066 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 606 "cfg.y" /* yacc.c:1646  */
    { memlog=(yyvsp[0].intval); }
#line 3072 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 607 "cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 3078 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 608 "cfg.y" /* yacc.c:1646  */
    { memdbg=(yyvsp[0].intval); }
#line 3084 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 609 "cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 3090 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 610 "cfg.y" /* yacc.c:1646  */
    { sip_warning=(yyvsp[0].intval); }
#line 3096 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 611 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3102 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 612 "cfg.y" /* yacc.c:1646  */
    { user=(yyvsp[0].strval); }
#line 3108 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 613 "cfg.y" /* yacc.c:1646  */
    { user=(yyvsp[0].strval); }
#line 3114 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 614 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3120 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 615 "cfg.y" /* yacc.c:1646  */
    { group=(yyvsp[0].strval); }
#line 3126 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 616 "cfg.y" /* yacc.c:1646  */
    { group=(yyvsp[0].strval); }
#line 3132 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 617 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3138 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 618 "cfg.y" /* yacc.c:1646  */
    { chroot_dir=(yyvsp[0].strval); }
#line 3144 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 619 "cfg.y" /* yacc.c:1646  */
    { chroot_dir=(yyvsp[0].strval); }
#line 3150 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 620 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3156 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 621 "cfg.y" /* yacc.c:1646  */
    { working_dir=(yyvsp[0].strval); }
#line 3162 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 622 "cfg.y" /* yacc.c:1646  */
    { working_dir=(yyvsp[0].strval); }
#line 3168 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 623 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3174 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 624 "cfg.y" /* yacc.c:1646  */
    { mhomed=(yyvsp[0].intval); }
#line 3180 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 625 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3186 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 626 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_disable=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3198 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 633 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3204 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 634 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_accept_aliases=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3216 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 641 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3222 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 642 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_children_no=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3234 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 649 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3240 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 650 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_connect_timeout=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3252 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 657 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3258 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 658 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_send_timeout=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3270 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 665 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3276 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 666 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_con_lifetime=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3288 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 673 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3294 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 674 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_poll_method=get_poll_type((yyvsp[0].strval));
			if (tcp_poll_method==POLL_NONE) {
				LOG(L_CRIT, "bad poll method name:"
						" %s\n, try one of %s.\n",
						(yyvsp[0].strval), poll_support);
				yyerror("bad tcp_poll_method "
						"value");
			}
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3313 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 688 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_poll_method=get_poll_type((yyvsp[0].strval));
			if (tcp_poll_method==POLL_NONE) {
				LOG(L_CRIT, "bad poll method name:"
						" %s\n, try one of %s.\n",
						(yyvsp[0].strval), poll_support);
				yyerror("bad tcp_poll_method "
						"value");
			}
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3332 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 702 "cfg.y" /* yacc.c:1646  */
    { yyerror("poll method name expected"); }
#line 3338 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 703 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_max_connections=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 3350 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 710 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3356 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 711 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_disable=(yyvsp[0].intval);
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 3368 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 718 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3374 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 719 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_disable=!((yyvsp[0].intval));
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 3386 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 726 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3392 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 727 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_log=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3404 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 734 "cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 3410 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 735 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_port_no=(yyvsp[0].intval);
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 3422 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 742 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3428 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 743 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv23;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3440 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 750 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv2;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3452 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 757 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv3;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3464 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 764 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_TLSv1;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3476 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 771 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			yyerror("SSLv23, SSLv2, SSLv3 or TLSv1 expected");
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3488 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 778 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_verify_cert=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3500 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 785 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3506 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 786 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_require_cert=(yyvsp[0].intval);
		#else
			warn( "tls-in-core support not compiled in");
		#endif
	}
#line 3518 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 793 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3524 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 794 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_cert_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3536 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 801 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3542 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 802 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_pkey_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3554 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 809 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3560 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 810 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_ca_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3572 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 817 "cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 3578 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 818 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_handshake_timeout=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3590 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 825 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3596 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 826 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_send_timeout=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 3608 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 833 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3614 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 834 "cfg.y" /* yacc.c:1646  */
    { server_signature=(yyvsp[0].intval); }
#line 3620 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 835 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3626 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 836 "cfg.y" /* yacc.c:1646  */
    { reply_to_via=(yyvsp[0].intval); }
#line 3632 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 837 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3638 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 838 "cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[0].sockid); lst_tmp; lst_tmp=lst_tmp->next) {
			if (add_listen_iface(lst_tmp->name, lst_tmp->port, lst_tmp->proto, 0)!=0) {
				LOG(L_CRIT,  "ERROR: cfg. parser: failed to add listen address\n");
				break;
			}
		}
	}
#line 3651 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 846 "cfg.y" /* yacc.c:1646  */
    { yyerror("ip address or hostname expected"); }
#line 3657 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 847 "cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[0].sockid); lst_tmp; lst_tmp=lst_tmp->next)
			add_alias(lst_tmp->name, strlen(lst_tmp->name), lst_tmp->port, lst_tmp->proto);
	}
#line 3666 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 851 "cfg.y" /* yacc.c:1646  */
    { yyerror(" hostname expected"); }
#line 3672 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 852 "cfg.y" /* yacc.c:1646  */
    {
		default_global_address.s=(yyvsp[0].strval);
		default_global_address.len=strlen((yyvsp[0].strval));
	}
#line 3681 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 856 "cfg.y" /* yacc.c:1646  */
    {yyerror("ip address or hostname expected"); }
#line 3687 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 857 "cfg.y" /* yacc.c:1646  */
    {
		tmp=int2str((yyvsp[0].intval), &i_tmp);
		if ((default_global_port.s=pkg_malloc(i_tmp))==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
			default_global_port.len=0;
		} else {
			default_global_port.len=i_tmp;
			memcpy(default_global_port.s, tmp, default_global_port.len);
		};
	}
#line 3702 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 867 "cfg.y" /* yacc.c:1646  */
    {yyerror("ip address or hostname expected"); }
#line 3708 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 868 "cfg.y" /* yacc.c:1646  */
    { disable_core_dump=(yyvsp[0].intval); }
#line 3714 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 869 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3720 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 870 "cfg.y" /* yacc.c:1646  */
    { open_files_limit=(yyvsp[0].intval); }
#line 3726 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 871 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3732 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 872 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast_loopback=(yyvsp[0].intval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 3744 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 879 "cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 3750 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 880 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast_ttl=(yyvsp[0].intval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 3762 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 887 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3768 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 888 "cfg.y" /* yacc.c:1646  */
    { tos=(yyvsp[0].intval); }
#line 3774 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 889 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3780 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 890 "cfg.y" /* yacc.c:1646  */
    { ser_kill_timeout=(yyvsp[0].intval); }
#line 3786 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 891 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3792 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 892 "cfg.y" /* yacc.c:1646  */
    { IF_STUN(stun_refresh_interval=(yyvsp[0].intval)); }
#line 3798 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 893 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3804 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 894 "cfg.y" /* yacc.c:1646  */
    { IF_STUN(stun_allow_stun=(yyvsp[0].intval)); }
#line 3810 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 895 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3816 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 896 "cfg.y" /* yacc.c:1646  */
    { IF_STUN(stun_allow_fp=(yyvsp[0].intval)) ; }
#line 3822 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 897 "cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 3828 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 898 "cfg.y" /* yacc.c:1646  */
    { yyerror("unknown config variable"); }
#line 3834 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 901 "cfg.y" /* yacc.c:1646  */
    {
		DBG("loading module %s\n", (yyvsp[0].strval));
			if (load_module((yyvsp[0].strval))!=0) {
				yyerror("failed to load module");
			}
	}
#line 3845 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 907 "cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 3851 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 908 "cfg.y" /* yacc.c:1646  */
    {
		if (set_mod_param_regex((yyvsp[-5].strval), (yyvsp[-3].strval), PARAM_STRING, (yyvsp[-1].strval)) != 0) {
			 yyerror("Can't set module parameter");
		}
	}
#line 3861 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 913 "cfg.y" /* yacc.c:1646  */
    {
		if (set_mod_param_regex((yyvsp[-5].strval), (yyvsp[-3].strval), PARAM_INT, (void*)(yyvsp[-1].intval)) != 0) {
			 yyerror("Can't set module parameter");
		}
	}
#line 3871 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 918 "cfg.y" /* yacc.c:1646  */
    { yyerror("Invalid arguments"); }
#line 3877 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 921 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 3883 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 922 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 3889 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 925 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.ipaddr)=pkg_malloc(sizeof(struct ip_addr));
		if ((yyval.ipaddr)==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
		} else {
			memset((yyval.ipaddr), 0, sizeof(struct ip_addr));
			(yyval.ipaddr)->af=AF_INET;
			(yyval.ipaddr)->len=4;
			if (((yyvsp[-6].intval)>255) || ((yyvsp[-6].intval)<0) ||
				((yyvsp[-4].intval)>255) || ((yyvsp[-4].intval)<0) ||
				((yyvsp[-2].intval)>255) || ((yyvsp[-2].intval)<0) ||
				((yyvsp[0].intval)>255) || ((yyvsp[0].intval)<0)) {
				yyerror("invalid ipv4 address");
				(yyval.ipaddr)->u.addr32[0]=0;
				/* $$=0; */
			} else {
				(yyval.ipaddr)->u.addr[0]=(yyvsp[-6].intval);
				(yyval.ipaddr)->u.addr[1]=(yyvsp[-4].intval);
				(yyval.ipaddr)->u.addr[2]=(yyvsp[-2].intval);
				(yyval.ipaddr)->u.addr[3]=(yyvsp[0].intval);
				/*
				$$=htonl( ($1<<24)|
				($3<<16)| ($5<<8)|$7 );
				*/
			}
		}
	}
#line 3921 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 954 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.ipaddr)=pkg_malloc(sizeof(struct ip_addr));
		if ((yyval.ipaddr)==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
		} else {
			memset((yyval.ipaddr), 0, sizeof(struct ip_addr));
			(yyval.ipaddr)->af=AF_INET6;
			(yyval.ipaddr)->len=16;
		#ifdef USE_IPV6
			if (inet_pton(AF_INET6, (yyvsp[0].strval), (yyval.ipaddr)->u.addr)<=0) {
				yyerror("bad ipv6 address");
			}
		#else
			yyerror("ipv6 address & no ipv6 support compiled in");
			YYABORT;
		#endif
		}
	}
#line 3944 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 974 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 3950 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 975 "cfg.y" /* yacc.c:1646  */
    {(yyval.ipaddr)=(yyvsp[-1].ipaddr); }
#line 3956 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 979 "cfg.y" /* yacc.c:1646  */
    {
					tmp=int2str((yyvsp[0].intval), &i_tmp);
					if (((yyval.strval)=pkg_malloc(i_tmp+1))==0) {
						yyerror("out of  memory");
						YYABORT;
					} else {
						memcpy((yyval.strval), tmp, i_tmp);
						(yyval.strval)[i_tmp]=0;
					}
						}
#line 3971 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 989 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 3977 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 990 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 3983 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 994 "cfg.y" /* yacc.c:1646  */
    { push((yyvsp[-1].action), &main_rt.rlist[DEFAULT_RT]); }
#line 3989 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 995 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=route_get(&main_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (main_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &main_rt.rlist[i_tmp]);
	}
#line 4006 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1007 "cfg.y" /* yacc.c:1646  */
    { yyerror("invalid  route  statement"); }
#line 4012 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1010 "cfg.y" /* yacc.c:1646  */
    {
									push((yyvsp[-1].action), &failure_rt.rlist[DEFAULT_RT]);
										}
#line 4020 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1013 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=route_get(&failure_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (failure_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &failure_rt.rlist[i_tmp]);
	}
#line 4037 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1025 "cfg.y" /* yacc.c:1646  */
    { yyerror("invalid failure_route statement"); }
#line 4043 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1028 "cfg.y" /* yacc.c:1646  */
    {
									push((yyvsp[-1].action), &onreply_rt.rlist[DEFAULT_RT]);
										}
#line 4051 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1031 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=route_get(&onreply_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (onreply_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &onreply_rt.rlist[i_tmp]);
	}
#line 4068 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1043 "cfg.y" /* yacc.c:1646  */
    { yyerror("invalid onreply_route statement"); }
#line 4074 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1046 "cfg.y" /* yacc.c:1646  */
    {
									push((yyvsp[-1].action), &branch_rt.rlist[DEFAULT_RT]);
										}
#line 4082 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1049 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=route_get(&branch_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (branch_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &branch_rt.rlist[i_tmp]);
	}
#line 4099 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1061 "cfg.y" /* yacc.c:1646  */
    { yyerror("invalid branch_route statement"); }
#line 4105 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1063 "cfg.y" /* yacc.c:1646  */
    {
									push((yyvsp[-1].action), &onsend_rt.rlist[DEFAULT_RT]);
												}
#line 4113 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1066 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=route_get(&onsend_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (onsend_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &onsend_rt.rlist[i_tmp]);
	}
#line 4130 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1078 "cfg.y" /* yacc.c:1646  */
    { yyerror("invalid onsend_route statement"); }
#line 4136 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1100 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_exp(LOGAND_OP, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 4142 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1101 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_exp(LOGOR_OP, (yyvsp[-2].expr), (yyvsp[0].expr));  }
#line 4148 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1102 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_exp(NOT_OP, (yyvsp[0].expr), 0);  }
#line 4154 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1103 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[-1].expr); }
#line 4160 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1104 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=(yyvsp[0].expr); }
#line 4166 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1107 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=EQUAL_OP; }
#line 4172 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1108 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=DIFF_OP; }
#line 4178 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1110 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=(yyvsp[0].intval); }
#line 4184 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1111 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=GT_OP; }
#line 4190 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1112 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=LT_OP; }
#line 4196 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1113 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=GTE_OP; }
#line 4202 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1114 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=LTE_OP; }
#line 4208 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1117 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval)= BINOR_OP; }
#line 4214 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1118 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = BINAND_OP; }
#line 4220 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1121 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=(yyvsp[0].intval); }
#line 4226 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1122 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=MATCH_OP; }
#line 4232 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1125 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=URI_O;}
#line 4238 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1126 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=FROM_URI_O;}
#line 4244 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1127 "cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=TO_URI_O;}
#line 4250 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1131 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)= mk_elem((yyvsp[-1].intval), METHOD_O, 0, STRING_ST, (yyvsp[0].strval));}
#line 4256 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1132 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), METHOD_O, 0, AVP_ST, (yyvsp[0].attr)); }
#line 4262 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1133 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), METHOD_O, 0, SELECT_ST, (yyvsp[0].select)); }
#line 4268 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1134 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), METHOD_O, 0, STRING_ST,(yyvsp[0].strval)); }
#line 4274 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1135 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("string expected"); }
#line 4280 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1136 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator,== , !=, or =~ expected"); }
#line 4286 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1137 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, STRING_ST, (yyvsp[0].strval)); }
#line 4292 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1138 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, STRING_ST, (yyvsp[0].strval)); }
#line 4298 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1139 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, AVP_ST, (yyvsp[0].attr)); }
#line 4304 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1140 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, SELECT_ST, (yyvsp[0].select)); }
#line 4310 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1141 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, MYSELF_ST, 0); }
#line 4316 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1142 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("string or MYSELF expected"); }
#line 4322 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1143 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, == , != or =~ expected"); }
#line 4328 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1145 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCPORT_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 4334 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1146 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCPORT_O, 0, AVP_ST, (void*)(yyvsp[0].attr) ); }
#line 4340 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1147 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4346 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1148 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("==, !=, <,>, >= or <=  expected"); }
#line 4352 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1150 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), DSTPORT_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 4358 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1151 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), DSTPORT_O, 0, AVP_ST, (void*)(yyvsp[0].attr) ); }
#line 4364 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1152 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4370 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1153 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("==, !=, <,>, >= or <=  expected"); }
#line 4376 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1155 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_port");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPORT_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) );
	}
#line 4385 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1159 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_port");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPORT_O, 0, AVP_ST, (void*)(yyvsp[0].attr) );
	}
#line 4394 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1163 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4400 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1164 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("==, !=, <,>, >= or <=  expected"); }
#line 4406 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1166 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_port");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOPORT_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) );
	}
#line 4415 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1170 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_port");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOPORT_O, 0, AVP_ST, (void*)(yyvsp[0].attr) );
	}
#line 4424 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1174 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4430 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1175 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("==, !=, <,>, >= or <=  expected"); }
#line 4436 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1177 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), PROTO_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 4442 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1178 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), PROTO_O, 0, AVP_ST, (void*)(yyvsp[0].attr) ); }
#line 4448 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1179 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("protocol expected (udp, tcp or tls)"); }
#line 4454 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1181 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4460 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1183 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_proto");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPROTO_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) );
	}
#line 4469 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1187 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_proto");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPROTO_O, 0, AVP_ST, (void*)(yyvsp[0].attr) );
	}
#line 4478 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1191 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("protocol expected (udp, tcp or tls)"); }
#line 4484 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1192 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4490 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1194 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), AF_O, 0, NUMBER_ST,(void *) (yyvsp[0].intval) ); }
#line 4496 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1195 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), AF_O, 0, AVP_ST,(void *) (yyvsp[0].attr) ); }
#line 4502 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1196 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4508 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1197 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4514 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1199 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_af");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDAF_O, 0, NUMBER_ST, (void *) (yyvsp[0].intval) ); }
#line 4522 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1202 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_af");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDAF_O, 0, AVP_ST, (void *) (yyvsp[0].attr) );
	}
#line 4531 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1206 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4537 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1207 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4543 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1209 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), MSGLEN_O, 0, NUMBER_ST, (void *) (yyvsp[0].intval) ); }
#line 4549 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1210 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), MSGLEN_O, 0, AVP_ST, (void *) (yyvsp[0].attr) ); }
#line 4555 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1211 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), MSGLEN_O, 0, NUMBER_ST, (void *) BUF_SIZE); }
#line 4561 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1212 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4567 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1213 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4573 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1215 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), RETCODE_O, 0, NUMBER_ST, (void *) (yyvsp[0].intval) ); }
#line 4579 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1216 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), RETCODE_O, 0, AVP_ST, (void *) (yyvsp[0].attr) ); }
#line 4585 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1217 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 4591 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1218 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("equal/!= operator expected"); }
#line 4597 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1220 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, NET_ST, (yyvsp[0].ipnet)); }
#line 4603 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1221 "cfg.y" /* yacc.c:1646  */
    {
		s_tmp.s=(yyvsp[0].strval);
		s_tmp.len=strlen((yyvsp[0].strval));
		ip_tmp=str2ip(&s_tmp);
		if (ip_tmp==0)
			ip_tmp=str2ip6(&s_tmp);
		if (ip_tmp) {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, NET_ST, mk_net_bitlen(ip_tmp, ip_tmp->len*8) );
		} else {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, STRING_ST, (yyvsp[0].strval));
		}
	}
#line 4620 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1233 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, STRING_ST, (yyvsp[0].strval)); }
#line 4626 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1234 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, MYSELF_ST, 0);
							}
#line 4633 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1236 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror( "ip address or hostname expected" ); }
#line 4639 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1237 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, ==, != or =~ expected");}
#line 4645 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1238 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem(	(yyvsp[-1].intval), DSTIP_O, 0, NET_ST, (void*)(yyvsp[0].ipnet)); }
#line 4651 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1239 "cfg.y" /* yacc.c:1646  */
    {
		s_tmp.s=(yyvsp[0].strval);
		s_tmp.len=strlen((yyvsp[0].strval));
		ip_tmp=str2ip(&s_tmp);
		if (ip_tmp==0)
			ip_tmp=str2ip6(&s_tmp);
		if (ip_tmp) {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), DSTIP_O, 0, NET_ST, mk_net_bitlen(ip_tmp, ip_tmp->len*8) );
		} else {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), DSTIP_O, 0, STRING_ST, (yyvsp[0].strval));
		}
	}
#line 4668 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1251 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem(	(yyvsp[-1].intval), DSTIP_O, 0, STRING_ST, (yyvsp[0].strval)); }
#line 4674 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1252 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem(	(yyvsp[-1].intval), DSTIP_O, 0, MYSELF_ST, 0); }
#line 4680 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1253 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror( "ip address or hostname expected" ); }
#line 4686 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1254 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, ==, != or =~ expected"); }
#line 4692 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1255 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, NET_ST, (yyvsp[0].ipnet));
	}
#line 4701 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1259 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_ip");
		s_tmp.s=(yyvsp[0].strval);
		s_tmp.len=strlen((yyvsp[0].strval));
		ip_tmp=str2ip(&s_tmp);
		if (ip_tmp==0)
			ip_tmp=str2ip6(&s_tmp);
		if (ip_tmp) {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, NET_ST, mk_net_bitlen(ip_tmp, ip_tmp->len*8) );
		} else {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, STRING_ST, (yyvsp[0].strval));
		}
	}
#line 4719 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1272 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, STRING_ST, (yyvsp[0].strval));
	}
#line 4728 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1276 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, MYSELF_ST, 0);
	}
#line 4737 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1280 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror( "ip address or hostname expected" ); }
#line 4743 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1281 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, ==, != or =~ expected"); }
#line 4749 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1282 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, NET_ST, (yyvsp[0].ipnet));
	}
#line 4758 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1286 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_ip");
		s_tmp.s=(yyvsp[0].strval);
		s_tmp.len=strlen((yyvsp[0].strval));
		ip_tmp=str2ip(&s_tmp);
		if (ip_tmp==0)
			ip_tmp=str2ip6(&s_tmp);
		if (ip_tmp) {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, NET_ST, mk_net_bitlen(ip_tmp, ip_tmp->len*8) );
		} else {
			(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, STRING_ST, (yyvsp[0].strval));
		}
	}
#line 4776 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1299 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, STRING_ST, (yyvsp[0].strval));
	}
#line 4785 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1303 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, MYSELF_ST, 0);
	}
#line 4794 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1307 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror( "ip address or hostname expected" ); }
#line 4800 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1308 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, ==, != or =~ expected"); }
#line 4806 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1310 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[0].intval), 0, MYSELF_ST, 0); }
#line 4812 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1311 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SRCIP_O, 0, MYSELF_ST, 0); }
#line 4818 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1312 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), DSTIP_O, 0, MYSELF_ST, 0); }
#line 4824 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1313 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("snd_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), SNDIP_O, 0, MYSELF_ST, 0);
	}
#line 4833 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1317 "cfg.y" /* yacc.c:1646  */
    {
		onsend_check("to_ip");
		(yyval.expr)=mk_elem((yyvsp[-1].intval), TOIP_O, 0, MYSELF_ST, 0);
	}
#line 4842 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1321 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror(" URI, SRCIP or DSTIP expected"); }
#line 4848 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1322 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror ("invalid operator, == or != expected"); }
#line 4854 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1323 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( NO_OP, ACTION_O, 0, ACTIONS_ST, (yyvsp[0].action));  }
#line 4860 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1324 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( NO_OP, NUMBER_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 4866 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1326 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( NO_OP, AVP_O, (void*)(yyvsp[0].attr), 0, 0); }
#line 4872 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1327 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( (yyvsp[-1].intval), AVP_O, (void*)(yyvsp[-2].attr), STRING_ST, (yyvsp[0].strval)); }
#line 4878 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1328 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( (yyvsp[-1].intval), AVP_O, (void*)(yyvsp[-2].attr), SELECT_ST, (yyvsp[0].select)); }
#line 4884 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1329 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( (yyvsp[-1].intval), AVP_O, (void*)(yyvsp[-2].attr), NUMBER_ST, (void*)(yyvsp[0].intval)); }
#line 4890 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1330 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( (yyvsp[-1].intval), AVP_O, (void*)(yyvsp[-2].attr), NUMBER_ST, (void*)(yyvsp[0].intval)); }
#line 4896 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1331 "cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem( (yyvsp[-1].intval), AVP_O, (void*)(yyvsp[-2].attr), AVP_ST, (void*)(yyvsp[0].attr)); }
#line 4902 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1333 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( NO_OP, SELECT_O, (yyvsp[0].select), 0, 0); }
#line 4908 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1334 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( (yyvsp[-1].intval), SELECT_O, (yyvsp[-2].select), STRING_ST, (yyvsp[0].strval)); }
#line 4914 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1335 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( (yyvsp[-1].intval), SELECT_O, (yyvsp[-2].select), AVP_ST, (void*)(yyvsp[0].attr)); }
#line 4920 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1336 "cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem( (yyvsp[-1].intval), SELECT_O, (yyvsp[-2].select), SELECT_ST, (yyvsp[0].select)); }
#line 4926 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1339 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=mk_net((yyvsp[-2].ipaddr), (yyvsp[0].ipaddr)); }
#line 4932 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1340 "cfg.y" /* yacc.c:1646  */
    {
		if (((yyvsp[0].intval)<0) || ((yyvsp[0].intval)>(yyvsp[-2].ipaddr)->len*8)) {
			yyerror("invalid bit number in netmask");
			(yyval.ipnet)=0;
		} else {
			(yyval.ipnet)=mk_net_bitlen((yyvsp[-2].ipaddr), (yyvsp[0].intval));
		/*
			$$=mk_net($1, htonl( ($3)?~( (1<<(32-$3))-1 ):0 ) );
		*/
		}
	}
#line 4948 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1351 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=mk_net_bitlen((yyvsp[0].ipaddr), (yyvsp[0].ipaddr)->len*8); }
#line 4954 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1352 "cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=0; yyerror("netmask (eg:255.0.0.0 or 8) expected"); }
#line 4960 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1355 "cfg.y" /* yacc.c:1646  */
    {(yyval.strval)=".";}
#line 4966 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1356 "cfg.y" /* yacc.c:1646  */
    {(yyval.strval)="-"; }
#line 4972 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1360 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 4978 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1361 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.strval)=(char*)pkg_malloc(strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))+1);
		if ((yyval.strval)==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: memory allocation failure while parsing host\n");
		} else {
			memcpy((yyval.strval), (yyvsp[-2].strval), strlen((yyvsp[-2].strval)));
			(yyval.strval)[strlen((yyvsp[-2].strval))]=*(yyvsp[-1].strval);
			memcpy((yyval.strval)+strlen((yyvsp[-2].strval))+1, (yyvsp[0].strval), strlen((yyvsp[0].strval)));
			(yyval.strval)[strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))]=0;
		}
		pkg_free((yyvsp[-2].strval));
		pkg_free((yyvsp[0].strval));
	}
#line 4996 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1374 "cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=0; pkg_free((yyvsp[-2].strval)); yyerror("invalid hostname"); }
#line 5002 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1378 "cfg.y" /* yacc.c:1646  */
    {
		/* check if allowed */
		if ((yyvsp[0].action) && rt==ONSEND_ROUTE) {
			switch((yyvsp[0].action)->type) {
				case DROP_T:
				case SEND_T:
				case SEND_TCP_T:
				case LOG_T:
				case SETFLAG_T:
				case RESETFLAG_T:
				case ISFLAGSET_T:
				case IF_T:
				case MODULE_T:
					(yyval.action)=(yyvsp[0].action);
					break;
				default:
					(yyval.action)=0;
					yyerror("command not allowed in onsend_route\n");
			}
		} else {
			(yyval.action)=(yyvsp[0].action);
		}
	}
#line 5030 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1403 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[0].action); }
#line 5036 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1404 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[0].action); }
#line 5042 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1405 "cfg.y" /* yacc.c:1646  */
    { (yyval.action) = (yyvsp[0].action); }
#line 5048 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1406 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[-1].action); }
#line 5054 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1409 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[0].action); }
#line 5060 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1410 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[-1].action); }
#line 5066 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1413 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=append_action((yyvsp[-1].action), (yyvsp[0].action)); }
#line 5072 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1414 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[0].action);}
#line 5078 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1415 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad command"); }
#line 5084 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1418 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[-1].action);}
#line 5090 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1419 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[0].action);}
#line 5096 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1420 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[-1].action);}
#line 5102 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1421 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0;}
#line 5108 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1422 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad command: missing ';'?"); }
#line 5114 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1425 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action( IF_T, 3, EXPR_ST, (yyvsp[-1].expr), ACTIONS_ST, (yyvsp[0].action), NOSUBTYPE, 0); }
#line 5120 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1426 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action( IF_T, 3, EXPR_ST, (yyvsp[-3].expr), ACTIONS_ST, (yyvsp[-2].action), ACTIONS_ST, (yyvsp[0].action)); }
#line 5126 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1435 "cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-1) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[0].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[0].strval));
		sel.n++;
	}
#line 5140 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1444 "cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-2) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-3].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-3].strval));
		sel.n++;
		sel.params[sel.n].type = SEL_PARAM_INT;
		sel.params[sel.n].v.i = (yyvsp[-1].intval);
		sel.n++;
	}
#line 5157 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1456 "cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-2) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-3].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-3].strval));
		sel.n++;
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-1].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-1].strval));
		sel.n++;
	}
#line 5175 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1475 "cfg.y" /* yacc.c:1646  */
    { sel.n = 0; sel.f[0] = 0; }
#line 5181 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1475 "cfg.y" /* yacc.c:1646  */
    {
		sel_ptr = (select_t*)pkg_malloc(sizeof(select_t));
		if (!sel_ptr) {
			yyerror("No memory left to allocate select structure\n");
		}
		memcpy(sel_ptr, &sel, sizeof(select_t));
		(yyval.select) = sel_ptr;
	}
#line 5194 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1485 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM; }
#line 5200 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1486 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO; }
#line 5206 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1487 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_URI; }
#line 5212 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1488 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_URI; }
#line 5218 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1489 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_USER; }
#line 5224 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1490 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_USER; }
#line 5230 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1491 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_DOMAIN; }
#line 5236 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1492 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_DOMAIN; }
#line 5242 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1493 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_ALL | AVP_CLASS_GLOBAL; }
#line 5248 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1496 "cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_NAME_STR; s_attr->name.s.s = (yyvsp[0].strval); s_attr->name.s.len = strlen ((yyvsp[0].strval)); }
#line 5254 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1503 "cfg.y" /* yacc.c:1646  */
    {
		s_attr = (struct avp_spec*)pkg_malloc(sizeof(struct avp_spec));
		if (!s_attr) { yyerror("No memory left"); }
		s_attr->type = 0;
	}
#line 5264 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1510 "cfg.y" /* yacc.c:1646  */
    { (yyval.attr) = s_attr; }
#line 5270 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1513 "cfg.y" /* yacc.c:1646  */
    {
		s_attr->type|= (AVP_NAME_STR | ((yyvsp[-1].intval)<0?AVP_INDEX_BACKWARD:AVP_INDEX_FORWARD));
		s_attr->index = ((yyvsp[-1].intval)<0?-(yyvsp[-1].intval):(yyvsp[-1].intval));
		(yyval.attr) = s_attr;
	}
#line 5280 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1520 "cfg.y" /* yacc.c:1646  */
    {
		s_attr->type|= AVP_INDEX_ALL;
		(yyval.attr) = s_attr;
	}
#line 5289 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1540 "cfg.y" /* yacc.c:1646  */
    {
		avp_spec_t *avp_spec;
		str s;
		int type, idx;
		avp_spec = pkg_malloc(sizeof(*avp_spec));
		if (!avp_spec) {
			yyerror("Not enough memory");
			YYABORT;
		}
		s.s = (yyvsp[0].strval);
		if (s.s[0] == '$')
			s.s++;
		s.len = strlen(s.s);
		if (parse_avp_name(&s, &type, &avp_spec->name, &idx)) {
			yyerror("error when parsing AVP");
		        pkg_free(avp_spec);
			YYABORT;
		}
		avp_spec->type = type;
		avp_spec->index = idx;
		(yyval.attr) = avp_spec;
	}
#line 5316 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1570 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = ASSIGN_T; }
#line 5322 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1573 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, AVP_ST, (yyvsp[-2].attr), STRING_ST, (yyvsp[0].strval)); }
#line 5328 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1574 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, AVP_ST, (yyvsp[-2].attr), NUMBER_ST, (void*)(yyvsp[0].intval)); }
#line 5334 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1575 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, AVP_ST, (yyvsp[-2].attr), ACTION_ST, (yyvsp[0].action)); }
#line 5340 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1576 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, AVP_ST, (yyvsp[-2].attr), AVP_ST, (yyvsp[0].attr)); }
#line 5346 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1577 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, AVP_ST, (void*)(yyvsp[-2].attr), SELECT_ST, (void*)(yyvsp[0].select)); }
#line 5352 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1578 "cfg.y" /* yacc.c:1646  */
    { (yyval.action) = mk_action((yyvsp[-3].intval), 2, AVP_ST, (yyvsp[-4].attr), EXPR_ST, (yyvsp[-1].expr)); }
#line 5358 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1581 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = 1; }
#line 5364 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1582 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = 0; }
#line 5370 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1583 "cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = -1; }
#line 5376 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1586 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5382 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1587 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5388 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1588 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); }
#line 5394 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1589 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5400 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1590 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5406 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1591 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5412 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1592 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); }
#line 5418 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1593 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5424 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1594 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); }
#line 5430 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1595 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5436 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1596 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward argument"); }
#line 5442 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1597 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5448 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1598 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5454 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1599 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); }
#line 5460 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1600 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5466 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1601 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5472 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1602 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5478 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1603 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); }
#line 5484 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1604 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5490 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1605 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); }
#line 5496 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1606 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5502 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1607 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward_udp argument"); }
#line 5508 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1608 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5514 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1609 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5520 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1610 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); }
#line 5526 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1611 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5532 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1612 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5538 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1613 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5544 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1614 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); }
#line 5550 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1615 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5556 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1616 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); }
#line 5562 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1617 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5568 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1618 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward_tcp argument"); }
#line 5574 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 449:
#line 1619 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0);
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5587 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1627 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0);
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5600 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 451:
#line 1635 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0);
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5613 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1643 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5626 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 453:
#line 1651 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5639 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1659 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
					}
#line 5652 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1667 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, URIPORT_ST, 0);
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5665 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1675 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5678 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 1683 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, NUMBER_ST, 0);
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 5691 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1691 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5697 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 1692 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward_tls argument"); }
#line 5703 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 460:
#line 1693 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5709 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1694 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5715 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1695 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); }
#line 5721 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1696 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5727 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1697 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(SEND_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5733 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1698 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5739 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1699 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5745 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1700 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad send argument"); }
#line 5751 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1701 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5757 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1702 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); }
#line 5763 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1703 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_TCP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); }
#line 5769 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1704 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	SEND_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval));}
#line 5775 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1705 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(SEND_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5781 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1706 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SEND_TCP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 5787 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1707 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5793 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1708 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad send_tcp argument"); }
#line 5799 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1709 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST, (void*)EXIT_R_F); }
#line 5805 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1710 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)(yyvsp[-1].intval), NUMBER_ST, (void*)EXIT_R_F); }
#line 5811 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1711 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)(yyvsp[0].intval), NUMBER_ST, (void*)EXIT_R_F); }
#line 5817 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1712 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, RETCODE_ST, 0, NUMBER_ST, (void*)EXIT_R_F); }
#line 5823 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1713 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST, (void*)EXIT_R_F); }
#line 5829 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1714 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)1, NUMBER_ST, (void*)RETURN_R_F); }
#line 5835 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1715 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)(yyvsp[0].intval), NUMBER_ST, (void*)RETURN_R_F);}
#line 5841 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1716 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, RETCODE_ST, 0, NUMBER_ST, (void*)RETURN_R_F);}
#line 5847 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1717 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST, (void*)RETURN_R_F); }
#line 5853 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1718 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(LOG_T, 2, NUMBER_ST, (void*)4, STRING_ST, (yyvsp[-1].strval)); }
#line 5859 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1719 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(LOG_T, 2, NUMBER_ST, (void*)(yyvsp[-3].intval), STRING_ST, (yyvsp[-1].strval)); }
#line 5865 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1720 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5871 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1721 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad log argument"); }
#line 5877 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1722 "cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(SETFLAG_T, 1, NUMBER_ST,
													(void*)(yyvsp[-1].intval));
									}
#line 5888 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1728 "cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(SETFLAG_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
									}
#line 5899 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1734 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 5905 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1735 "cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(RESETFLAG_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
									}
#line 5915 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1740 "cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(RESETFLAG_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
									}
#line 5926 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1746 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 5932 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1747 "cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(ISFLAGSET_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
									}
#line 5942 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1752 "cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(ISFLAGSET_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
									}
#line 5953 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1758 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 5959 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1759 "cfg.y" /* yacc.c:1646  */
    {
		i_tmp=get_avpflag_no((yyvsp[-1].strval));
		if (i_tmp==0) yyerror("avpflag not declared");
		(yyval.action)=mk_action(AVPFLAG_OPER_T, 3, AVP_ST, (yyvsp[-3].attr), NUMBER_ST, (void*)(long)i_tmp, NUMBER_ST, (void*)(yyvsp[-5].intval));
	}
#line 5969 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1764 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 5975 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1765 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(ERROR_T, 2, STRING_ST, (yyvsp[-3].strval), STRING_ST, (yyvsp[-1].strval)); }
#line 5981 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1766 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 5987 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 1767 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad error argument"); }
#line 5993 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 1768 "cfg.y" /* yacc.c:1646  */
    {
						i_tmp=route_get(&main_rt, (yyvsp[-1].strval));
						if (i_tmp==-1){
							yyerror("internal error");
							YYABORT;
						}
						(yyval.action)=mk_action(ROUTE_T, 1, NUMBER_ST,(void*)(long)i_tmp);
										}
#line 6006 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 1776 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6012 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 1777 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad route argument"); }
#line 6018 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 1778 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(EXEC_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6024 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 1779 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_HOST_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6030 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 1780 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6036 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 509:
#line 1781 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6042 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 1782 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(PREFIX_T, 1, STRING_ST,  (yyvsp[-1].strval)); }
#line 6048 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 1783 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6054 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 512:
#line 1784 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6060 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 513:
#line 1785 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(STRIP_TAIL_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval)); }
#line 6066 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 1786 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6072 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 1787 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, number expected"); }
#line 6078 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 516:
#line 1788 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(STRIP_T, 1, NUMBER_ST, (void*) (yyvsp[-1].intval)); }
#line 6084 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 1789 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6090 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 1790 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, number expected"); }
#line 6096 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 1791 "cfg.y" /* yacc.c:1646  */
    {
		qvalue_t q;
		if (str2q(&q, (yyvsp[-1].strval), strlen((yyvsp[-1].strval))) < 0) {
			yyerror("bad argument, q value expected");
		}
		(yyval.action)=mk_action(APPEND_BRANCH_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void *)(long)q);
	}
#line 6108 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 1798 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(APPEND_BRANCH_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, (void *)Q_UNSPECIFIED); }
#line 6114 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 1799 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(APPEND_BRANCH_T, 2, STRING_ST, 0, NUMBER_ST, (void *)Q_UNSPECIFIED); }
#line 6120 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 1800 "cfg.y" /* yacc.c:1646  */
    {  (yyval.action)=mk_action( APPEND_BRANCH_T, 1, STRING_ST, 0); }
#line 6126 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 1801 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_HOSTPORT_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6132 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 1802 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6138 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 1803 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6144 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 1804 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_PORT_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6150 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 1805 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6156 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 1806 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6162 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 1807 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_USER_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6168 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 1808 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6174 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 1809 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6180 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 1810 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_USERPASS_T, 1, STRING_ST, (yyvsp[-1].strval)); }
#line 6186 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 1811 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6192 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 1812 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6198 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 1813 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_URI_T, 1, STRING_ST,(yyvsp[-1].strval)); }
#line 6204 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 1814 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6210 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 1815 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6216 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 538:
#line 1816 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(REVERT_URI_T, 0); }
#line 6222 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 539:
#line 1817 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(REVERT_URI_T, 0); }
#line 6228 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 540:
#line 1818 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORCE_RPORT_T, 0); }
#line 6234 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 541:
#line 1819 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORCE_RPORT_T, 0); }
#line 6240 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 542:
#line 1820 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 6252 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 543:
#line 1827 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 0);
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 6264 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 544:
#line 1834 "cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 0);
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 6276 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 545:
#line 1841 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("bad argument, number expected"); }
#line 6282 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 546:
#line 1842 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		if ((str_tmp=pkg_malloc(sizeof(str)))==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
		} else {
			str_tmp->s=(yyvsp[-1].strval);
			str_tmp->len=strlen((yyvsp[-1].strval));
			(yyval.action)=mk_action(SET_ADV_ADDR_T, 1, STR_ST, str_tmp);
		}
	}
#line 6297 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 547:
#line 1852 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6303 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 548:
#line 1853 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6309 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 549:
#line 1854 "cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		tmp=int2str((yyvsp[-1].intval), &i_tmp);
		if ((str_tmp=pkg_malloc(sizeof(str)))==0) {
			LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
		} else {
			if ((str_tmp->s=pkg_malloc(i_tmp))==0) {
				LOG(L_CRIT, "ERROR: cfg. parser: out of memory.\n");
			} else {
				memcpy(str_tmp->s, tmp, i_tmp);
				str_tmp->len=i_tmp;
				(yyval.action)=mk_action(SET_ADV_PORT_T, 1, STR_ST, str_tmp);
			}
		}
	}
#line 6329 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 550:
#line 1869 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 6335 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 551:
#line 1870 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6341 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 552:
#line 1871 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORCE_SEND_SOCKET_T, 1, SOCKID_ST, (yyvsp[-1].sockid)); }
#line 6347 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 553:
#line 1872 "cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, [proto:]host[:port] expected"); }
#line 6353 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 554:
#line 1873 "cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 6359 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 555:
#line 1874 "cfg.y" /* yacc.c:1646  */
    {mod_func_action = mk_action(MODULE_T, 2, MODEXP_ST, NULL, NUMBER_ST, 0); }
#line 6365 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 556:
#line 1874 "cfg.y" /* yacc.c:1646  */
    {
		mod_func_action->val[0].u.data = find_export_record((yyvsp[-4].strval), mod_func_action->val[1].u.number, rt);
		if (mod_func_action->val[0].u.data == 0) {
			if (find_export_record((yyvsp[-4].strval), mod_func_action->val[1].u.number, 0) ) {
					yyerror("Command cannot be used in the block\n");
			} else {
				yyerror("unknown command, missing loadmodule?\n");
			}
			pkg_free(mod_func_action);
			mod_func_action=0;
		}
		(yyval.action) = mod_func_action;
	}
#line 6383 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 558:
#line 1890 "cfg.y" /* yacc.c:1646  */
    { }
#line 6389 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 559:
#line 1891 "cfg.y" /* yacc.c:1646  */
    {}
#line 6395 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 560:
#line 1892 "cfg.y" /* yacc.c:1646  */
    { yyerror("call params error\n"); YYABORT; }
#line 6401 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 561:
#line 1895 "cfg.y" /* yacc.c:1646  */
    {
		if (mod_func_action->val[1].u.number < MAX_ACTIONS-2) {
			mod_func_action->val[mod_func_action->val[1].u.number+2].type = NUMBER_ST;
			mod_func_action->val[mod_func_action->val[1].u.number+2].u.number = (yyvsp[0].intval);
			mod_func_action->val[1].u.number++;
		} else {
			yyerror("Too many arguments\n");
		}
	}
#line 6415 "cfg.tab.c" /* yacc.c:1646  */
    break;

  case 562:
#line 1904 "cfg.y" /* yacc.c:1646  */
    {
		if (mod_func_action->val[1].u.number < MAX_ACTIONS-2) {
			mod_func_action->val[mod_func_action->val[1].u.number+2].type = STRING_ST;
			mod_func_action->val[mod_func_action->val[1].u.number+2].u.string = (yyvsp[0].strval);
			mod_func_action->val[1].u.number++;
		} else {
			yyerror("Too many arguments\n");
		}
	}
#line 6429 "cfg.tab.c" /* yacc.c:1646  */
    break;


#line 6433 "cfg.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1914 "cfg.y" /* yacc.c:1906  */


extern int line;
extern int column;
extern int startcolumn;
static void warn(char* s)
{
	LOG(L_WARN, "cfg. warning: (%d,%d-%d): %s\n", line, startcolumn,
			column, s);
	cfg_warnings++;
}

static void yyerror(char* s)
{
	LOG(L_CRIT, "parse error (%d,%d-%d): %s\n", line, startcolumn,
			column, s);
	cfg_errors++;
}


static struct socket_id* mk_listen_id(char* host, int proto, int port)
{
	struct socket_id* l;
	l=pkg_malloc(sizeof(struct socket_id));
	if (l==0) {
		LOG(L_CRIT,"ERROR: cfg. parser: out of memory.\n");
	} else {
		l->name=host;
		l->port=port;
		l->proto=proto;
		l->next=0;
	}
	return l;
}


/*
int main(int argc, char ** argv)
{
	if (yyparse()!=0)
		fprintf(stderr, "parsing error\n");
}
*/
