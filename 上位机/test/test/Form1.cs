using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;  //接口
using System.Data.OleDb;    //数据库
namespace test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            myport = new SerialPort();
            myport.PortName = "com3";
            myport.BaudRate = 9600;
            myport.StopBits = StopBits.One;
            myport.DataBits = 8;
            myport.Parity = Parity.None;
        }

        SerialPort myport = null;
        int cflag=0;
        string ss = "";
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
 
        }

        private void button1_Click(object sender, EventArgs e)
        {
       
            myport.Open();
        }

        private void button2_Click(object sender, EventArgs e)
        {


            byte[] send_data= new byte[6];
            send_data[0]= 0x11;
            send_data[1]= 0xf1;
            send_data[2]= 0xf1;
            send_data[3] = 0xf1;
            send_data[4] = 0xf1;
            send_data[5] = 0x11;

            myport.Write(send_data, 0, 6);
        }

        private void button3_Click(object sender, EventArgs e)
        {
  //          byte[] data = new byte[6];
 //           myport.Read(data,0,6);
   //         string ss = byteToHexStr(data);

            while (cflag == 0)
            {
                byte[] data = new byte[8];
                byte[] rdata = new byte[6];
                myport.Read(data, 0, 8);
                if ((data[6] == 0x33) && (data[7] == 0x44))
                {
                    for (int i = 0; i <= 5; i++)
                    {
                        rdata[i] = data[i];
                    }
                    cflag = 1;
                }
                ss = byteToHexStr(rdata);

            }

            textBox1.Text = ss;
            //    MessageBox.Show("","");
    //        user form = new user();
    //        form.Show();
        }




        public static string byteToHexStr(byte[] bytes)
        {
            string returnStr = "";
            if (bytes != null)
            {
                for (int i = 0; i < bytes.Length; i++)
                {
                    returnStr += bytes[i].ToString("X2");
                }
            }
            return returnStr;
        }

        private void button4_Click(object sender, EventArgs e)
        {
           
              //生成链接数据库字符串
                         string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myIC.mdb";
             //创建数据库连接
                        OleDbConnection conn = new OleDbConnection(conStr);
             //打开数据库连接
                       conn.Open();

                       OleDbDataReader myReader = null;
              //sql语句        
                       string sql = "select * from myTable where cstr(卡号)="+textBox2.Text;
              //命令
                       OleDbCommand cmd = new OleDbCommand(sql, conn);

                    myReader = cmd.ExecuteReader();
                       if (myReader.Read())
                       {
                             textBox1.Text = myReader.GetString(0);
                             textBox2.Text = Convert.ToString(myReader.GetDecimal(3));  
                     
                       }
                 myReader.Close();
                 conn.Close();

        }

        private void button5_Click(object sender, EventArgs e)
        {
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();

            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "insert into myTable(卡号) values('"+textBox1.Text+"')";
            //命令
            OleDbCommand command = new OleDbCommand(sql, conn);
            command.ExecuteNonQuery();
            conn.Close();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            string mm = textBox1.Text;
            byte[] myMoney = new byte[8];
            myMoney[0] = 0xaa;
            myMoney[5] = 0x00;
            myMoney[6] = 0x33;
            myMoney[7] = 0x44;
            for (int x = 0; x <= 3; x++)
            {
                if (mm[x] == '0')
                {
                    myMoney[x + 1] = 0x30;
                }
                if (mm[x] == '1')
                {
                    myMoney[x + 1] = 0x31;
                }
                if (mm[x] == '2')
                {
                    myMoney[x + 1] = 0x32;
                }
                if (mm[x] == '3')
                {
                    myMoney[x + 1] = 0x33;
                }
                if (mm[x] == '4')
                {
                    myMoney[x + 1] = 0x34;
                }
                if (mm[x] == '5')
                {
                    myMoney[x + 1] = 0x35;
                }
                if (mm[x] == '6')
                {
                    myMoney[x + 1] = 0x36;
                }
                if (mm[x] == '7')
                {
                    myMoney[x + 1] = 0x37;
                }
                if (mm[x] == '8')
                {
                    myMoney[x + 1] = 0x38;
                }
                if (mm[x] == '9')
                {
                    myMoney[x + 1] = 0x39;
                }
            }
           
                myport.Write(myMoney, 0, 8);
            //      myport.Open();
            //     display(myMoney);
            //     myport.Close();
        }
        private void display(byte[] mm)
        {

            //         if (!myport.IsOpen)
            //          {
            //             myport.Open();
            //         }

            int dflag = 1;
            int j = 0;
            while (dflag == 1)
            {

                byte[] send_order = new byte[8];
                send_order[0] = mm[0];
                send_order[1] = mm[1];
                send_order[2] = mm[2];
                send_order[3] = mm[3];
                send_order[4] = mm[4];
                send_order[5] = mm[5];
                send_order[6] = mm[6];
                send_order[7] = mm[7];


                myport.Write(send_order, 0, 8);
                j++;
                if (j >= 100)
                {
                    dflag = 0;
                }

            }
            //          myport.Close();

        }

        private void button7_Click(object sender, EventArgs e)
        {
           // pictureBox1.Visible = true;
           // pictureBox1.ImageLocation = textBox1.Text + ".jpg";
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
           // pictureBox1.Visible = false;
        }
    }
}
