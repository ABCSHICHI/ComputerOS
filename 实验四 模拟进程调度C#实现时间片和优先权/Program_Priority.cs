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
            public float priority;//优先数
            public int needTime;//需要运行的时间
            public int runTime;//已运行时间
            public int waitTime;//等待时间
            public PCB next;//链接到下一个PCB
            public override string ToString()
            {
                string s = string.Format("进程名：{0}，\t优先数：{1:F}，\t等待时间：{2}，\t还需要时间：{3}\t",
                        name,
                        priority,
                        waitTime,
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

            private PCB head = null;//头节点
            //插入链表:头插入
            public void Input(PCB pcb)
            {
                if(null == head)
                {
                    head = pcb;
                }
                else
                {
                    pcb.next = head.next;
                    head.next = pcb;
                }
                count++;
            }



            //等待时间+1，计算优先权
            public void CalculationPriority()
            {
                //获得当前链表
                PCB current = head;
                while (current != null)
                {
                    current.waitTime++;
                    current.priority = (float)(current.waitTime + current.needTime) / current.needTime;
                    current = current.next;
                }
            }
            

            ////获取优先权最大的进程
            public PCB GetMaxPriorityProcess()
            {

                PCB max,ancher,temp;
                temp = head;
                max = head;
                ancher = head;
                //找到优先权最大项
                while (temp.next != null)
                {
                    if (temp.next.priority > max.priority)
                    {
                        max = temp.next;
                        ancher = temp;
                        temp = temp.next;
                    }
                    else
                        temp = temp.next;
                }
                //摘除最大项
                //当最大的是头节点的时候，将头节点设为原来的头节点的下一个。
                if (max == head)
                    head = head.next;
                //当最大的是尾节点的时候，将锚点的下一个设置为空
                else if (max.next == null)
                    ancher.next = null;
                else
                    ancher.next = ancher.next.next;
                //调用进程前，为剩余的进程添加等待时间，并更新优先权
                CalculationPriority();
                count--;
                return max;
            }
            //计算平均周转时间和带权周转时间
            public void CalculateTime()
            {
                PCB turnTimeNode = head;
                int turnTime = 0;
                while (turnTimeNode != null)
                {
                    turnTime += (turnTimeNode.waitTime + turnTimeNode.needTime);//周转时间
                    turnTimeNode = turnTimeNode.next;
                }
                PCB weightTurnTimeNode = head;
                float weightTurnTime = 0.0f;
                while (weightTurnTimeNode != null)
                {
                    weightTurnTime += weightTurnTimeNode.priority;//周转时间
                    weightTurnTimeNode = weightTurnTimeNode.next;
                }

                Console.WriteLine("平均周转时间：{0:F},平均带权周转时间：{1:F}", 
                    turnTime / count, 
                    weightTurnTime / count);
                
            }


            public void PrintList()
            {
                //输出就绪队列里所有节点的信息
                PCB current = head;
                while (current != null)
                {
                    Console.WriteLine(current.ToString());
                    current = current.next;
                }
            }
        }
        static PCBList finish = new PCBList();//完成进程队列
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
                    pcb.next = null;
                    pcb.name = pcbInfo[0];
                    pcb.state = State.ready;
                    pcb.priority = 0.0f;
                    pcb.needTime = Convert.ToInt32(pcbInfo[2]);
                    pcb.waitTime = 0;
                    pcb.runTime = 0;
                    pcb.next = null;
                    //将新的pcb插入就绪队列
                    ready.Input(pcb);

                }
                sr.Close();
                fs.Close();
            }
            //try函数结尾
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        //执行pcb进程的一个时间片，执行完后修改pcb
        static void Run(PCB pcb)
        {
            //执行完一个时间片后，优先数-1
            //pcb.priority--;
            //已执行时间+1
            pcb.runTime++;
            pcb.next = null;
            if (pcb.runTime == pcb.needTime)
            {
                //如果执行时间等于需要时间，进程执行完毕
                pcb.state = State.finish;
                //Console.WriteLine("----------===========================------------");
                finish.Input(pcb);//插入完成队列
                //Console.WriteLine("填入完成链表的是：");
                //finish.PrintList();
                //Console.WriteLine("----------===========================------------");
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
            while (ready.Count != 0)//就绪队列非空
            {
                //从就绪队列中摘取队首进程
                PCB currentPCB = ready.GetMaxPriorityProcess();
                //currentPCB.waitTime--;
                Run(currentPCB);
                Console.WriteLine("当前运行进程信息：");
                Console.WriteLine(currentPCB);
                Console.WriteLine("就绪队列列表：");
                ready.PrintList();
                if (currentPCB.state == State.finish)
                {
                    
                    Console.WriteLine("进程{0}执行完毕", currentPCB.name);
                    
                }
                else
                {
                    ready.Input(currentPCB);//插入就绪队列
                }
                Console.WriteLine("按任意键继续");
                Console.ReadKey();

            }
            Console.WriteLine("所有进程执行完成");

            finish.CalculateTime();
        }
    }
}
