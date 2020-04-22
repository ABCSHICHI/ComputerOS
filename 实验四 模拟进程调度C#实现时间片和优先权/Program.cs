using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace WS27Schedule
{
    class Program
    {
        public enum State
        {//进程的状态
            ready,
            run,
            finish
        }
        public class PCB
        {
            public string name;//进程名
            public State state;//进程状态
            public int priority;//优先数
            public int needTime;//需要运行的时间
            public int runTime;//已运行时间
            public int waitTime;//等待时间
            public PCB next;//链接到下一个PCB
            public override string ToString()
            {
                string s = string.Format("进程名：{0}，优先数：{1}，已运行时间：{2}，还需要时间：{3}",
                        name,
                        priority,
                        runTime,
                        needTime - runTime);
                return s;
            }
        }
        //创建就绪队列，队列中每个节点就是一个进程项PCB的信息
        //就绪队列按照PCB的优先数排列
        public class PCBList
        {
            private int count;//就绪长度
            public int Count
            {
                get { return count; }
            }
            private PCB head = null;//就绪队列头节点
            //将pcb按其优先数大小插入至就绪队列中
            public void Insert(PCB pcb)
            {
                //队列为空，令头节点指向pcb
                if (head == null)
                {
                    head = pcb;
                    count++;
                    return;
                }
                //比较pcb与队首 节点的优先数
                if(head.priority < pcb.priority)
                {
                    //如果pcb的优先数高于队首优先数
                    pcb.next = head;
                    head = pcb;
                    count++;
                    return;
                }
                //pcb不在队首。下面依次查找pcb插入位置
                PCB previous = head;
                PCB current = head.next;
                //从就绪队列第二个节点开始比较
                while (current != null)
                {
                    //队列第二个节点开始
                    //依次遍历队列节点的优先数与pcb的优先数
                    //直到碰到第一个节点的优先数 小于 pcb的优先数
                    if (current.priority < pcb.priority)
                        break;
                    previous = current;
                    current = current.next;
                }
                //跳出循环，应该在previous节点和current节点之间插入pcb
                previous.next = pcb;
                pcb.next = current;
                count++;
            }

            //摘取就绪队列的队首节点
            public PCB GetFirst()
            {
                PCB pcb = head;
                head = head.next;
                pcb.next = null;
                count--;
                return pcb;
            }

            public void PrintList()
            {
                //输出就绪队列里所有节点的信息
                PCB current = head;
                while(current != null)
                {
                    Console.WriteLine(current.ToString());
                    current = current.next;
                }
            }
        }
        static PCBList ready = new PCBList();//就绪进程队列
        static void InputFromFile(string filename)
        {
            try
            {
                FileStream fs = new FileStream(filename, FileMode.Open);
                StreamReader sr = new StreamReader(fs);
                //读取进程
                int n = Convert.ToInt32(sr.ReadLine());
                string[] pcbInfo;
                //依次读取每个进程
                for (int i = 0; i < n; i++)
                {
                    pcbInfo = sr.ReadLine().Split();
                    //建立、初始化PCB
                    PCB pcb = new PCB();
                    pcb.name = pcbInfo[0];
                    pcb.state = State.ready;
                    pcb.priority = Convert.ToInt32(pcbInfo[1]);
                    pcb.needTime = Convert.ToInt32(pcbInfo[2]);
                    pcb.runTime = 0;
                    pcb.next = null;
                    //将新的pcb插入就绪队列
                    ready.Insert(pcb);

                }
                sr.Close();
                fs.Close();
            }
            //try函数结尾
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        //执行pcb进程的一个时间片，执行完后修改pcb
        static void Run(PCB pcb) {
            //执行完一个时间片后，优先数-1
            pcb.priority--;
            //已执行时间+1
            pcb.runTime++;
            if(pcb.runTime == pcb.needTime)
            {
                //如果执行时间等于需要时间，进程执行完毕
                pcb.state = State.finish;
            }
            else
            {
                //还没执行完，将pcd插入队列
                pcb.state = State.ready;
            }
        }
        static void Main(string[] args)
        {
            InputFromFile("pcblist.txt");//读入pcblist
            //模拟调度
            int timeSlice = 0;//时间片计数
            while(ready.Count != 0)//就绪队列非空
            {
                timeSlice++;//轮转下一个时间片
                Console.WriteLine("运行时间片{0}", timeSlice);
                //从就绪队列中摘取队首进程
                PCB currentPCB = ready.GetFirst();
                Run(currentPCB);
                Console.WriteLine("当前运行进程信息：");
                Console.WriteLine(currentPCB);
                Console.WriteLine("就绪队列列表：");
                ready.PrintList();
                if(currentPCB.state == State.finish)
                {
                    Console.WriteLine("进程{0}执行完毕", currentPCB.name);
                }
                else
                {
                    ready.Insert(currentPCB);
                }
                Console.WriteLine("按任意键继续");
                Console.ReadKey();

            }
            Console.WriteLine("所有进程执行完成");
        }
    }
}
