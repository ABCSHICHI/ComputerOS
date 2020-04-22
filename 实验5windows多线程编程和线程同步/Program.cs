using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
namespace WS27MultiThread
{
    class Program
    {
        static void WriteX()
        {
            for (int i = 0; i < 300; i++)
            {
                Console.Write(i+"x  ");
            }
        }
        static void Main(string[] args)
        {
            //创建一个线程，其执行的方法是WriteX函数
            Thread thread = new Thread(WriteX);
            //启动线程
            thread.Start();
            //主线程循环输出300个y
            for (int i = 0; i < 300; i++)
            {
                Console.Write(i+"y  ");

            }
        }
    }
}
