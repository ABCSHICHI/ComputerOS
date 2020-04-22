using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
namespace WS27MultiThread
{
    class program1
    {
        static volatile int ticket = 100;
        static Mutex mutex = new Mutex();//创建一个互斥类
        static void Sell()
        {
           
            while(true){
                mutex.WaitOne();//等待操作
                if(ticket>0)
                {
                    ticket--;
                    Console.WriteLine("{0} is running,ticket ={1}", Thread.CurrentThread.Name, ticket);
                }
                else
                {
                    mutex.ReleaseMutex();
                    break;
                }
                //Thread.Sleep(500);
                 mutex.ReleaseMutex();
            }
           
        }
        static void Main(string[] args)
        {
            //指定thread1执行
            Thread thread1 = new Thread(Sell);
            //设置thread1的名称，用来区分不同的线程，便于调试
            thread1.Name = "线程1";
            //启动线程
            thread1.Start();
            //再新建一个线程，同样执行Sell函数
            Thread thread2 = new Thread(Sell);
            thread2.Name = "线程2";
            thread2.Start();
            //让主线程挂起，知道thread1和thread2完成位置
            thread1.Join();
            thread2.Join();
            Console.WriteLine("售票完毕，主线程退出");

        }
    }
}
