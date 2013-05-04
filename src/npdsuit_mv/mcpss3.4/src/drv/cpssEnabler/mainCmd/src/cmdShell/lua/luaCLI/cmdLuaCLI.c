/*************************************************************************
* cmdLuaCLI.c
*
* DESCRIPTION:
*       a LUA CLI implementation
*
* DEPENDENCIES:
*
* COMMENTS:
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
**************************************************************************/

/***** Include files ***************************************************/

#include <cmdShell/common/cmdCommon.h>
#include <cmdShell/os/cmdStream.h>
#include <cmdShell/os/cmdExecute.h>

#ifndef ALU_LUA
# include <lua.h>
# include <lualib.h>
# include <lauxlib.h>
# include <string.h>
#else /* ALU_LUA */

# define CMD_LUA_FS
# include "../lua-5.1/src/lua.h"
# include "../lua-5.1/src/lualib.h"
# include "../lua-5.1/src/lauxlib.h"

# define cmdOsMalloc    malloc
# define cmdOsFree      free
# define cmdOsMemCpy    memcpy
# define cmdOsMemCmp    memcmp
# define cmdOsMemSet    memset
# define cmdOsSprintf   sprintf

#endif

#include <cmdShell/lua/cmdLuaCommon.h>

/***** Defines  ********************************************************/
#define CLI_BANNER  "***************************************\n"     \
                    " LUA CLI shell ready\n"     \
                    "***************************************\n\n"

#define CLI_PROMPT  ">"
#define CLI_READLINE_FLAG_HISTORY       1
#define CLI_READLINE_FLAG_COMPLETION    2
#define IS_CLI_CMD_EXIT(line)       \
        (luaCLI_strncmp(line,"CLIexit", 7) == 0 \
          && (line[7] == 0 || line[7] == ' '))
#ifdef LUACLI_MEMSTAT
#define IS_CLI_CMD_MEMSTAT(line)        \
        (luaCLI_strncmp(line,"CLImemstat", 10) == 0 \
          && (line[10] == 0 || line[10] == ' '))
#endif /* LUACLI_MEMSTAT */
#define IS_CLI_CMD_LUA(line)        \
        (luaCLI_strncmp(line,"lua", 3) == 0     \
          && (line[3] == 0 || line[3] == ' '))

#define LUA_PROMPT          "lua>"
#define LUA_PROMPT_CONT     "lua>>"

#define CLI_MAX_LINE_LEN 300


#if 0
int check_wraplCpssCommon();
int check_wraplCpssDxChVlan();
int check_wraplCpssDxChBrg();
int check_wraplCpssDxChMirror();
int check_wraplCpssDxChNet();
int check_cmdLuaBitwizeFunctions();
int check_wraplCpssTrunk();
int check_wraplCpssPFC();
int check_wraplCpssQCN();  
int check_wraplCpssDeviceConfig();
#endif

/***** Private declarations ********************************************/
static int luaCLI_ReadLine
(
    cmdStreamPTR    IOStream,
    lua_State       *L,
    const char      *prompt,
    int             flags,
    char            *line,
    int             length
);

static int check_files()
{
#if 0
    check_wraplCpssCommon();
    check_wraplCpssDxChVlan();
    check_wraplCpssDxChBrg();
    check_wraplCpssDxChMirror();
    check_wraplCpssDxChNet();
    check_wraplCpssTrunk();
    check_cmdLuaBitwizeFunctions();
    check_wraplCpssPFC(); 
    check_wraplCpssQCN();
    check_wraplCpssDeviceConfig(); /* to resolve link issue */
#endif
    return 1;
};



/***** Public Functions ************************************************/

/*******************************************************************************
* luaCLI_strncmp
*
* DESCRIPTION:
*       strncmp
*
* INPUTS:
*       s1      - string
*       s2      - string
*       n       - max no of chars to compare
*
* OUTPUTS:
*       None
*
* RETURNS:
*       0       - if strings equal
*       1       - if not equal
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int luaCLI_strncmp(const char *s1, const char *s2, int n)
{
    for (;n;n--,s1++,s2++)
    {
        if (*s1 == *s2)
        {
            if (*s1 == 0)
                return 0;
            continue;
        }
        return 1;
    }
    return 0;
}

/*******************************************************************************
* luaCLI_readLine
*
* DESCRIPTION:
*       Read line
*
* INPUTS:
*       L       - lua_State
*        1st    - string: prompt, default '?'
*        2nd    - bool: history enable flag, default disable
*
* OUTPUTS:
*       luaPtr
*
* RETURNS:
*       string
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int luaCLI_readLine(lua_State *L)
{
    cmdStreamPTR IOStream;
    const char *prompt = "?";
    int flags = 0;
    char    line[CLI_MAX_LINE_LEN];

    lua_getglobal(L, "IOStream");
    IOStream = (cmdStreamPTR)lua_touserdata(L, -1);
    lua_pop(L, 1);
    if (IOStream == NULL)
        return 0;

    if (lua_gettop(L) >= 1 && lua_isstring(L, 1))
    {
        prompt = lua_tostring(L, 1);
    }
    if (lua_gettop(L) >= 2 && lua_toboolean(L, 2))
    {
        flags |= CLI_READLINE_FLAG_HISTORY;
    }
    if (luaCLI_ReadLine(IOStream, L, prompt, flags, line, sizeof(line)) < 0)
    {
        return 0;
    }
    lua_pushstring(L, line);
    return 1;
}

/*******************************************************************************
* luaCLI_init
*
* DESCRIPTION:
*       Init Lua CLI
*
* INPUTS:
*       IOStream
*       memstat     - pointer to memstat structure or NULL
*
* OUTPUTS:
*       luaPtr
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_STATUS luaCLI_init
(
    cmdStreamPTR IOStream,
    lua_State **luaPtr,
    void *ud
)
{
    lua_State *L;

    L = cmdLua_newstate(ud);
    *luaPtr = L;

    /* set output stream */
    lua_pushlightuserdata(L, IOStream);
    lua_setglobal(L, "IOStream");

    lua_register(L, "cmdLuaCLI_readLine", luaCLI_readLine);

    lua_pushstring(L, CLI_PROMPT);
    lua_setglobal(L, "cmdLuaCLI_prompt");

    lua_newtable(L);
    lua_setglobal(L, "cmdLuaCLI_history");

    /* load libLuaCliDefs.lua */
    lua_getglobal(L, "cmdLuaCLI_require");
    lua_pushstring(L, "cmdLuaCLIDefs");
    lua_call(L, 1, 0);

    /*Don't call cmdLuaCLI_change_mode(): this should be done in cmdLuaCLIDefs */
#if 0
    /* set default mode */
    lua_getglobal(L, "cmdLuaCLI_change_mode");
    if (lua_isfunction(L, -1))
    {
        lua_pushstring(L, "");
        if (lua_pcall(L, 1, 0, 0) != 0)
        {
            lua_pushcfunction(L, luaPrint);
            lua_insert(L, -2);
            lua_pushstring(L, "Failed to execute cmdLuaCLI_change_mode(\"\"): ");
            lua_insert(L, -2);
            lua_concat(L, 2);
            lua_call(L, 1, 0);
        }
    }
    else
    {
        lua_pop(L, 1);
    }
#endif

    /*******************/
    check_files();
    /*******************/
    return GT_OK;
}

#define KEY_ORDINAL_CHAR(key) (((key) & 0xffffff00) == 0)
#define KEY_ENTER       0x0100
#define KEY_CANCEL      0x0200
#define KEY_COMPLETE    0x0300
#define KEY_SHOWCOMPL   0x0400
#define KEY_ERACE       0x0500
#define KEY_DELETE      0x0600
#define KEY_LEFT        0x0700
#define KEY_RIGHT       0x0800
#define KEY_PREV        0x0900
#define KEY_NEXT        0x0a00
#define KEY_FIRST       0x0b00
#define KEY_LAST        0x0c00
#define KEY_HELP        0x0d00


/*******************************************************************************
* luaCLI_readkey
*
* DESCRIPTION:
*       read a key from tty, parse key sequences to logical keys
*
* INPUTS:
*       IOStream
*
* OUTPUTS:
*       None
*
* RETURNS:
*       -1      - on error
*       key
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int luaCLI_readkey(cmdStreamPTR IOStream)
{
    int state;
    /* VT100 key sequences */
    struct {
        char    *seq;
        int     key;
    } keyseq[] = {
        { "\033",   KEY_CANCEL },
        { "\t",     KEY_COMPLETE },
        { "\010",   KEY_ERACE },    /* backspace */
        { "\177",   KEY_ERACE },
        { "\04",    KEY_DELETE },   /* ctrl-D */
        { "\033[D", KEY_LEFT },     /* left */
        { "\033[C", KEY_RIGHT },    /* right */
        { "\033[A", KEY_PREV },     /* up */
        { "\020",   KEY_PREV },     /* Ctrl-P */
        { "\033[B", KEY_NEXT },     /* down */
        { "\016",   KEY_NEXT },     /* ctrl-N */
        { "\01",    KEY_FIRST },    /* ctrl-A */
        { "\05",    KEY_LAST },     /* ctrl-E */
        { "\06",    KEY_SHOWCOMPL },/* ctrl-F */
        { "\07",    KEY_HELP },     /* ctrl-G */
        { NULL,     0 }
    };
    char buf[10];
    int ret, k, matches;


    state = 0;
    while (IOStream->connected(IOStream))
    {
        ret = IOStream->readChar(IOStream, buf+state, state ? 500 : -1);
        if (ret < 0)
        {
            /* error */
            return -1;
        }
        if (ret > 0)
        {
            if (state == 0)
            {
                /* Handle both CR and LF as KEY_ENTER. Ignore LF after CR */
                if (buf[0] == '\r')
                {
                    IOStream->wasCR = GT_TRUE;
                    return KEY_ENTER;
                }
                if (buf[0] == '\n')
                {
                    if (IOStream->wasCR == GT_TRUE)
                    {
                        IOStream->wasCR = GT_FALSE;
                        /* skip LF after CR */
                        continue;
                    }
                    IOStream->wasCR = GT_FALSE;
                    return KEY_ENTER;
                }
            }
            IOStream->wasCR = GT_FALSE;
            state++;
        }
        matches = 0;
        for (k = 0; keyseq[k].seq; k++)
        {
            if (cmdOsMemCmp(keyseq[k].seq, buf, state) == 0)
            {
                matches++;
            }
        }
        if (matches == 0)
        {
            if (state == 1)
                return (int)((unsigned char)buf[0]);
            return '?';
        }
        if (matches == 1)
        {
            for (k = 0; keyseq[k].seq; k++)
            {
                if (cmdOsMemCmp(keyseq[k].seq, buf, state) == 0)
                    break;
            }
            if (keyseq[k].seq[state] == 0)
                return keyseq[k].key;
            /* read till end of seq */
            continue;
        }
        /* matches > 1 */
        if (ret > 0)
            continue;
        /* timeout */
        buf[state] = 0;
        for (k = 0; keyseq[k].seq; k++)
        {
            /* one key matched */
            if (cmdOsMemCmp(keyseq[k].seq, buf, state + 1) == 0)
                return keyseq[k].key;
        }
        /* sequence not completed */
        return '?';
    }
    return -1;
}

/*******************************************************************************
* writeSeq1
*
* DESCRIPTION:
*       Write control sequence to terminal with single parameter
*
* INPUTS:
*       IOStream
*       fmt
*       param
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*       None
*
*******************************************************************************/
static void writeSeq1(
    cmdStreamPTR    IOStream,
    const char      *fmt,
    int             param
)
{
    char buf[20];
    int  l;
    l = cmdOsSprintf(buf, fmt, param);
    IOStream->write(IOStream, buf, l);
}

/*******************************************************************************
* shiftRight
*
* DESCRIPTION:
*       shift the line right 'count' chars from positon 'p'
*       shift line on terminal
*
* INPUTS:
*       IOStream
*       line
*       len
*       l
*       p
*       count
*
* OUTPUTS:
*       line
*       l
*
* RETURNS:
*       0   if error
*       1   if successful
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int shiftRight(
    cmdStreamPTR    IOStream,
    char            *line,
    int             len,
    int             *l,
    int             p,
    int             count
)
{
    int k;
    if (count <= 0)
    {
        return 0;
    }
    if (*l + count > len)
    {
        return 0;
    }
    if (p > *l)
    {
        return 0;
    }
    if (p == *l)
    {
        *l += count;
        /* pad with spaces */
        for (k = 0; k < count; k++)
            line[p+k] = ' ';
        return 1;
    }
    /* shift string */
    for (k = *l-1; k >= p; k--)
        line[k+count] = line[k];
    *l += count;
    /* pad with spaces */
    for (k = 0; k < count; k++)
        line[p+k] = ' ';
    /* shift tty */
    writeSeq1(IOStream, "\033[%d@", count);
    return 1;
}

/*******************************************************************************
* shiftLeft
*
* DESCRIPTION:
*       shift the line left 'count' chars to positon 'p'
*       shift line on terminal
*
* INPUTS:
*       IOStream
*       line
*       len
*       l
*       p
*       count
*
* OUTPUTS:
*       line
*       l
*
* RETURNS:
*       0   if error
*       1   if successful
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int shiftLeft(
    cmdStreamPTR    IOStream,
    char            *line,
    int             len,
    int             *l,
    int             p,
    int             count
)
{
    int k;
    if (count <= 0)
    {
        return 0;
    }
    if (p + count > *l)
    {
        return 0;
    }
    /* shift string */
    for (k = p; k + count < *l; k++)
        line[k] = line[k+count];
    *l -= count;
    /* shift tty */
    writeSeq1(IOStream, "\033[%dP", count);
    return 1;
}

/*******************************************************************************
* luaCLI_ReadLine
*
* DESCRIPTION:
*       readline implementation including the following
*           - line editing
*           - autocompletion
*           - history (not implemented yet)
*
* INPUTS:
*       IOStream
*       L       - lua_State
*       prompt
*       flags
*       length
*
* OUTPUTS:
*       line
*
* RETURNS:
*       <0  if error
*       line length
*
* COMMENTS:
*       None
*
*******************************************************************************/
static int luaCLI_ReadLine
(
    cmdStreamPTR    IOStream,
    lua_State       *L,
    const char      *prompt,
    int             flags,
    char            *line,
    int             length
)
{
    int     key;
    int     p,l;
    int     ncompl;
    int     history_len = 0;
    int     history_pos = 0;
    p = l = 0;
    ncompl = 0;

    IOStream->setTtyMode(IOStream, 0);

    /*! put current prompt */
    if (prompt != NULL)
    {
        IOStream->writeLine(IOStream, prompt);
    }
    else
    {
        lua_getglobal(L, "cmdLuaCLI_prompt");
        if (lua_type(L, -1) == LUA_TSTRING)
        {
            IOStream->write(IOStream, lua_tostring(L, -1), lua_objlen(L, -1));
        }
        else
        {
            prompt = CLI_PROMPT;
            IOStream->writeLine(IOStream, prompt);
        }
        lua_pop(L, 1);
    }

    if (flags & CLI_READLINE_FLAG_HISTORY)
    {
        lua_getglobal(L, "cmdLuaCLI_history");
        history_len = lua_objlen(L, -1);
        lua_pop(L, 1);
        history_pos = history_len + 1;
    }

    while (IOStream->connected(IOStream) && p < length)
    {
        key = luaCLI_readkey(IOStream);
        if (key < 0)
        {
            IOStream->setTtyMode(IOStream, 1);
            return -1;
        }
        if (key == 0)
            continue;
        if (key == KEY_COMPLETE && (flags & CLI_READLINE_FLAG_COMPLETION) != 0)
        {
            ncompl++;
            if (ncompl == 2)
                key = KEY_SHOWCOMPL;
        } else {
            ncompl = 0;
        }
        if (key == KEY_ENTER)
        {
            IOStream->write(IOStream, "\r\n", 2);
            break;
        }
#define BELL() IOStream->write(IOStream, "\a",1)
        if (KEY_ORDINAL_CHAR(key) && key == '?' && p == l)
        {
            /* ? typed at the last position enforces help */
            key = KEY_HELP;
        }
        if (KEY_ORDINAL_CHAR(key))
        {
            /* ordinal character */
            if (!shiftRight(IOStream, line, length, &l, p, 1))
            {
                BELL();
                continue;
            }
            line[p++] = (char)key;
            IOStream->write(IOStream, line + p - 1, 1);
            continue;
        }
        switch (key) {
            case KEY_ERACE:
                if (p)
                {
                    if (p == l)
                    {
                        p--;
                        l--;
                        IOStream->write(IOStream, "\b \b", 3);
                    }
                    else
                    {
                        p--;
                        IOStream->write(IOStream, "\b", 1);
                        if (!shiftLeft(IOStream, line, length, &l, p, 1))
                        {
                            BELL();
                        }
                    }
                }
                break;
            case KEY_DELETE:
                if (!shiftLeft(IOStream, line, length, &l, p, 1))
                {
                    BELL();
                }
                break;
            case KEY_LEFT:
                if (p)
                {
                    p--;
                    IOStream->write(IOStream, "\033[D",3);
                }
                else
                {
                    BELL();
                }
                break;
            case KEY_RIGHT:
                if (p < l)
                {
                    p++;
                    IOStream->write(IOStream, "\033[C",3);
                }
                else
                {
                    BELL();
                }
                break;
            case KEY_FIRST:
                if (p)
                {
                    writeSeq1(IOStream, "\033[%dD",p);
                    p = 0;
                }
                else
                {
                    BELL();
                }
                break;
            case KEY_LAST:
                if (p < l)
                {
                    writeSeq1(IOStream, "\033[%dC",l-p);
                    p = l;
                }
                else
                {
                    BELL();
                }
                break;
            case KEY_PREV:
            case KEY_NEXT:
                if ((flags & CLI_READLINE_FLAG_HISTORY) == 0)
                {
                    BELL();
                    break;
                }
                if ((key == KEY_PREV && history_pos - 1 > 0)
                    || (key == KEY_NEXT && history_pos <= history_len))
                {
                    const char *s;
                    if (p)
                    {
                        writeSeq1(IOStream, "\033[%dD",p);
                        p = 0;
                    }
                    IOStream->write(IOStream, "\033[0K", 4);
                    lua_getglobal(L, "cmdLuaCLI_history");
                    lua_pushlstring(L, line, l);
                    lua_rawseti(L, -2, history_pos);
                    if (key == KEY_PREV)
                    {
                        history_pos--;
                    }
                    else
                    {
                        history_pos++;
                    }
                    lua_rawgeti(L, -1, history_pos);
                    l = lua_objlen(L, -1);
                    s = lua_tostring(L, -1);
                    for (p = 0; p < l; p++)
                        line[p] = s[p];
                    lua_pop(L, 2);
                    IOStream->write(IOStream, line, p);
                }
                else
                {
                    BELL();
                }
                break;
#if 0
#define KEY_CANCEL
#endif
            case KEY_COMPLETE:
                if ((flags & CLI_READLINE_FLAG_COMPLETION) == 0)
                {
                    BELL();
                    break;
                }
                lua_getglobal(L, "cmdLuaCLI_complete");
                if (lua_isfunction(L, -1))
                {
                    int status;
                    lua_pushlstring(L, line, p);
                    status = lua_pcall(L, 1, 2, 0);
                    if (status && !lua_isnil(L, -1))
                    {
                        /* any error while loading or executing */
                        const char *msg = lua_tostring(L, -1);
                        if (msg == NULL) msg = "(error object is not a string)";
                        IOStream->writeLine(IOStream, "Lua error: ");
                        IOStream->writeLine(IOStream, msg);
                        IOStream->writeLine(IOStream, "\n");
                        lua_pop(L, 1);
                        break;
                    }
                }
                else
                {
                    lua_pop(L, 1);
                    lua_pushliteral(L, "");
                    lua_newtable(L);
                }
                /* string expected */
                if (!lua_isstring(L, -2))
                {
                    lua_pop(L, 2);
                    BELL();
                    continue;
                }
                if (lua_objlen(L, -2) > 0)
                {
                    const char    *s;
                    int     k, n;

                    n = lua_objlen(L, -2);
                    if (!shiftRight(IOStream, line, length, &l, p, n))
                    {
                        lua_pop(L, 2);
                        BELL();
                        continue;
                    }
                    s = lua_tostring(L, -2);
                    for (k = 0; k < n; k++)
                        line[p + k] = s[k];
                    IOStream->write(IOStream, line + p, n);
                    p += n;
                    if (lua_objlen(L, -1) < 2)
                    {
                        ncompl = 0;
                    }
                    lua_pop(L, 2);
                    break;
                }

                lua_pop(L, 2);
                BELL();
                break;
            case KEY_SHOWCOMPL:
                if ((flags & CLI_READLINE_FLAG_COMPLETION) == 0)
                {
                    BELL();
                    break;
                }
                lua_getglobal(L, "cmdLuaCLI_complete");
                if (lua_isfunction(L, -1))
                {
                    int status;
                    lua_pushlstring(L, line, p);
                    status = lua_pcall(L, 1, 2, 0);
                    if (status && !lua_isnil(L, -1))
                    {
                        /* any error while loading or executing */
                        const char *msg = lua_tostring(L, -1);
                        if (msg == NULL) msg = "(error object is not a string)";
                        IOStream->writeLine(IOStream, "Lua error: ");
                        IOStream->writeLine(IOStream, msg);
                        IOStream->writeLine(IOStream, "\n");
                        lua_pop(L, 1);
                        break;
                    }
                }
                else
                {
                    lua_pop(L, 1);
                    lua_pushliteral(L, "");
                    lua_newtable(L);
                }
                /* table expected */
                if (!lua_istable(L, -1))
                {
                    lua_pop(L, 2);
                    BELL();
                    continue;
                }
                if (lua_objlen(L, -1) < 1)
                {
                    lua_pop(L, 2);
                    BELL();
                    continue;
                }
                IOStream->write(IOStream, "\r\n", 2);
                lua_pushnil(L);  /* first key */
                while (lua_next(L, -2) != 0)
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    IOStream->write(IOStream, "\t",1);
                    IOStream->write(IOStream, lua_tostring(L, -1), lua_objlen(L, -1));
                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }

                lua_pop(L, 2);

                IOStream->write(IOStream, "\r\n", 2);
                /*! put current prompt */
                if (prompt != NULL)
                {
                    IOStream->writeLine(IOStream, prompt);
                }
                else
                {
                    lua_getglobal(L, "cmdLuaCLI_prompt");
                    IOStream->write(IOStream, lua_tostring(L, -1), lua_objlen(L, -1));
                    lua_pop(L, 1);
                }
                IOStream->write(IOStream, line, l);
                if (p < l)
                    writeSeq1(IOStream, "\033[%dD", l-p);
                break;

            case KEY_HELP:
                if ((flags & CLI_READLINE_FLAG_COMPLETION) == 0)
                {
                    BELL();
                    break;
                }
                lua_getglobal(L, "cmdLuaCLI_help");
                if (lua_isfunction(L, -1))
                {
                    int status;
                    lua_pushlstring(L, line, p);
                    status = lua_pcall(L, 1, 1, 0);
                    if (status && !lua_isnil(L, -1))
                    {
                        /* any error while loading or executing */
                        const char *msg = lua_tostring(L, -1);
                        if (msg == NULL) msg = "(error object is not a string)";
                        IOStream->writeLine(IOStream, "?\r\nLua error: ");
                        IOStream->writeLine(IOStream, msg);
                        IOStream->writeLine(IOStream, "\n");
                        lua_pop(L, 1);
                        break;
                    }
                }
                else
                {
                    lua_pop(L, 1);
                    lua_pushliteral(L, "");
                }
                if (lua_objlen(L, -1) <= 0)
                {
                    /* no help */
                    lua_pop(L, 1);
                    BELL();
                    break;
                }
                /* help received */
                IOStream->write(IOStream, "?\r\n", 3);
                /* string.gsub(s, '\n', '\r\n') */
                lua_getglobal(L, "string");
                lua_getfield(L, -1, "gsub");
                lua_remove(L, -2);
                lua_insert(L, -2);
                lua_pushliteral(L, "\n");
                lua_pushliteral(L, "\r\n");
                lua_call(L, 3, 1);
                IOStream->write(IOStream, lua_tostring(L, -1), lua_objlen(L, -1));
                lua_pop(L, 1);
                IOStream->write(IOStream, "\r\n", 2);
                /*! put current prompt */
                if (prompt != NULL)
                {
                    IOStream->writeLine(IOStream, prompt);
                }
                else
                {
                    lua_getglobal(L, "cmdLuaCLI_prompt");
                    IOStream->write(IOStream, lua_tostring(L, -1), lua_objlen(L, -1));
                    lua_pop(L, 1);
                }
                IOStream->write(IOStream, line, l);
                if (p < l)
                    writeSeq1(IOStream, "\033[%dD", l-p);
                break;

            default:
                {
                    char aa[40];
                    sprintf(aa, "<KEY=0x%04x>", key);
                    IOStream->writeLine(IOStream, aa);
                }
        }
    }
    IOStream->setTtyMode(IOStream, 1);

    if (l < length)
    {
        line[l] = 0;
    }
    /* if not empty push line to history */
    if (l > 0 && (flags & CLI_READLINE_FLAG_HISTORY) != 0)
    {
        lua_getglobal(L, "cmdLuaCLI_history");
        lua_pushlstring(L, line, l);
        lua_rawseti(L, -2, history_len + 1);
        lua_pop(L, 1);
    }
    return l;
}

/*******************************************************************************
* lua_status_incomplete
*
* DESCRIPTION:
*       
*
* INPUTS:
*       L       - lua_State
*       status  - return status of luaL_loadbuffer()
*
* OUTPUTS:
*       None
*
* RETURNS:
*       1  if status == incomplete
*       0 otherwise
*
* COMMENTS:
*       need to be updated to don't use strstr()
*
*******************************************************************************/
static int lua_status_incomplete
(
    lua_State   *L,
    int         status
)
{
    if (status == LUA_ERRSYNTAX)
    {
        size_t lmsg;
        const char *msg = lua_tolstring(L, -1, &lmsg);
        const char *tp = msg + lmsg - (sizeof(LUA_QL("<eof>")) - 1);
        if (strstr(msg, LUA_QL("<eof>")) == tp)
        {
            lua_pop(L, 1);
            return 1;
        }
    }
    return 0;  /* else... */
}

/*******************************************************************************
* luaCLI_EventLoop
*
* DESCRIPTION:
*       
*
* INPUTS:
*       stream  - pointer to stream descriptor
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_STATUS luaCLI_EventLoop
(
    cmdStreamPTR IOStream
)
{
    char    line[CLI_MAX_LINE_LEN];
    lua_State *L;
    enum { STATE_CLI_E, STATE_LUA_E } state = STATE_CLI_E;
    int status;
    int lua_line_idx = 0;
#ifdef  LUACLI_MEMSTAT
    LUACLI_MEM_STAT_STC memStat;
#endif /* LUACLI_MEMSTAT */

#ifdef  LUACLI_MEMSTAT
    cmdOsMemSet(&memStat, 0, sizeof(memStat));
    luaCLI_init(IOStream, &L, &memStat);
#else /* !LUACLI_MEMSTAT */
    luaCLI_init(IOStream, &L, NULL);
#endif /* LUACLI_MEMSTAT */

#ifdef CLI_BANNER
    IOStream->writeLine(IOStream, CLI_BANNER);
#endif

    /* accept and handle input as long as connected */
    while (IOStream->connected(IOStream))
    {
        if (state == STATE_CLI_E)
        {
            if (luaCLI_ReadLine(IOStream, L,
                NULL /* Lua's cmdLuaCLI_prompt */,
                CLI_READLINE_FLAG_HISTORY | CLI_READLINE_FLAG_COMPLETION,
                line, sizeof(line)) < 0)
            {
                /* input faled, possible connection failed */
                break;
            }
            
            if (!line[0])
            {
                continue;
            }


            if (IS_CLI_CMD_EXIT(line))
                break;
#ifdef  LUACLI_MEMSTAT
            if (IS_CLI_CMD_MEMSTAT(line))
            {
                cmdOsSprintf(line, "Memory statistic:\n\ttotalUsed=%d\n\tnChunks=%d\n",
                        memStat.totalUsed, memStat.nChunks);
                IOStream->writeLine(IOStream, line);
                continue;
            }
#endif /* LUACLI_MEMSTAT */
            if (IS_CLI_CMD_LUA(line))
            {
                lua_pushliteral(L, "");
                lua_line_idx = lua_gettop(L);
                state = STATE_LUA_E;
                continue;
            }
            /* execute line */
            lua_getglobal(L, "cmdLuaCLI_execute");
            if (lua_isfunction(L, -1))
            {
                int status;
                lua_pushstring(L, line);
                status = lua_pcall(L, 1, 0, 0);
                if (status && !lua_isnil(L, -1))
                {
                    /* any error while loading or executing */
                    const char *msg = lua_tostring(L, -1);
                    if (msg == NULL) msg = "(error object is not a string)";
                    IOStream->writeLine(IOStream, "Lua error: ");
                    IOStream->writeLine(IOStream, msg);
                    IOStream->writeLine(IOStream, "\n");
                    lua_pop(L, 1);
                }
            }
            else
            {
                lua_pop(L, 1);
            }
        }
        if (state == STATE_LUA_E)
        {
            if (luaCLI_ReadLine(IOStream, L,
                lua_strlen(L, -1) == 0
                    ? LUA_PROMPT
                    : LUA_PROMPT_CONT,
                CLI_READLINE_FLAG_HISTORY,
                line, sizeof(line)) < 0)
            {
                /* input faled, possible connection failed */
                break;
            }
            if (line[0] == '.' && line[1] == 0)
            {
                /* single dot at line, end lua mode */
                lua_settop(L, lua_line_idx);
                lua_pop(L, 1);
                state = STATE_CLI_E;
                continue;
            }
            if (lua_strlen(L, -1) > 0)
            {
                lua_pushliteral(L, "\n");
                lua_concat(L, 2);
            }
            lua_pushstring(L, line);
            lua_concat(L, 2);
            status = luaL_loadbuffer(L,
                        lua_tostring(L, lua_line_idx),
                        lua_strlen(L, lua_line_idx),
                        "=stdin");
            if (lua_status_incomplete(L, status))
            {
                /* line incomplete, do next line */
                continue;
            }
            if (status == 0)
            {
                status = lua_pcall(L, 0, 0, 0);
            }
            if (status && !lua_isnil(L, -1))
            {
                /* any error while loading or executing */
                const char *msg = lua_tostring(L, -1);
                if (msg == NULL) msg = "(error object is not a string)";
                IOStream->writeLine(IOStream, "Lua error: ");
                IOStream->writeLine(IOStream, msg);
                IOStream->writeLine(IOStream, "\n");
            }

            lua_settop(L, lua_line_idx);
            lua_pop(L, 1);
            lua_pushliteral(L, "");
        }
    }
    
    lua_close(L);

    return GT_OK;
}
