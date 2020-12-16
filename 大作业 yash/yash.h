#define MAXARGS 50                                //cmdline args
#define ARGLEN 100                                //token length
#define CLOSE "\001\033[0m\002"                   // 关闭所有属性
#define BLOD "\001\033[1m\002"                    // 强调、加粗、高亮
#define BEGIN(x, y) "\001\033[" #x ";" #y "m\002" // x: 背景，y: 前景

void ShowHistory();          //调用readline库函数写的函数
void ShowJobs();
int Fg(char *argv);
void Bg(char *argv);
void Sg(char *argv);
void Init();                 //加入环境变量；除了exit，不能用ctrl+c来退出yash
int Process(char **args);    //exec前的判断，判断是控制语句，还是要执行的命令
int Execute(char *argv[],int flag);   //exec函数，执行命令，fork + exec
char **SplitLine(char *str); //命令解析，拆！
void FreeList(char **list);  //把字符串数组tokens里的元素都free掉
void Fatal(char *s1, char *s2, int n);//出大问题啊，这不应该出现的
int BuiltinCommand(char **args, int *resultp);
int VLstore(char *name, char *val);
int VLexport(char *name);
void VLlist();
char *VLlookup(char *name);
int VLenviron2table(char *env[]);
char **VLtable2environ();
void pipeHandler(char *args[]);