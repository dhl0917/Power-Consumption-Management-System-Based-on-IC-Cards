using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;//接口
using System.Data.OleDb;    //数据库


namespace test
{
    public partial class login : Form
    {

        public login()
        {
            InitializeComponent();
            //      SerialPort myport = null;
            myport = new SerialPort();      //初始化myport
            myport.PortName = "com3";
            myport.BaudRate = 9600;
            myport.StopBits = StopBits.One;
            myport.DataBits = 8;
            myport.Parity = Parity.None;

        }
        SerialPort myport = null;
        public static string tString;
        int cflag = 0;
        string ss = "";
 //       int warning = 0;
 //       int ttflag = 0;
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
  
        }

        public static string byteToHexStr(byte[] bytes)     //16进制byte转string
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


        private void textBox1_Click(object sender, EventArgs e)
        {
            
        }

        private void textBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {

            if (myport.IsOpen)
            {
                while (cflag == 0)          //接受下位机卡号
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
                        ss = byteToHexStr(rdata);
                    }

                }
                textBox1.Text = ss;

                //生成链接数据库字符串
                string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                //创建数据库连接
                OleDbConnection conn = new OleDbConnection(conStr);
                //打开数据库连接
                conn.Open();
                OleDbDataReader myReader = null;
                //sql语句        
                string sql = "select * from myTable where cstr(卡号)='" + ss + "'";
                //命令
                OleDbCommand cmd = new OleDbCommand(sql, conn);
                myReader = cmd.ExecuteReader();

                if (myReader.Read())    //是否查到卡号
                {
                    if (myReader.GetBoolean(3)) //是否挂失
                    {

                        www();
                        MessageBox.Show("此卡已挂失", "Warning");
                        
                        cflag = 0;
                        ID.Visible = false;
                        textBox1.Visible = false;
                        button1.Visible = true;
                        textBox1.Text = "";
                    }
                    else
                    {

                        if (myReader.GetBoolean(1)) //是否管理员
                        {

                            MessageBox.Show("请点击管理员登录", "Warning");
                            cflag = 0;
                            myport.Close();
                            ID.Visible = false;
                            textBox1.Visible = false;
                            button1.Visible = true;
                            textBox1.Text = "";
                        }
                        else
                        {
                            //把余额转后后发送给下位机
                          decimal money = myReader.GetDecimal(4);
                            if (money >= 100)
                            {
                                string mm = Convert.ToString(money * 10);
                                byte[] myMoney = new byte[8];
                                myMoney[0] = 0xaa;
                                myMoney[5] = 0x00;
                                myMoney[6] = 0x33;
                                myMoney[7] = 0x44;
                                for (int x = 0; x <= 3; x++)
                                {
                                    if (mm[x] == '0')
                                    {
                                        myMoney[4 - x] = 0x30;
                                    }
                                    if (mm[x] == '1')
                                    {
                                        myMoney[4 - x] = 0x31;
                                    }
                                    if (mm[x] == '2')
                                    {
                                        myMoney[4 - x] = 0x32;
                                    }
                                    if (mm[x] == '3')
                                    {
                                        myMoney[4 - x] = 0x33;
                                    }
                                    if (mm[x] == '4')
                                    {
                                        myMoney[4 - x] = 0x34;
                                    }
                                    if (mm[x] == '5')
                                    {
                                        myMoney[4 - x] = 0x35;
                                    }
                                    if (mm[x] == '6')
                                    {
                                        myMoney[4 - x] = 0x36;
                                    }
                                    if (mm[x] == '7')
                                    {
                                        myMoney[4 - x] = 0x37;
                                    }
                                    if (mm[x] == '8')
                                    {
                                        myMoney[4 - x] = 0x38;
                                    }
                                    if (mm[x] == '9')
                                    {
                                        myMoney[4 - x] = 0x39;
                                    }
                                }
                                myport.Write(myMoney, 0, 8);
                            }

                            tString = textBox1.Text;        //传递给user窗口

                            user form = new user();
                            form.ShowDialog();
                            this.Hide();
                            myport.Close();
                            cflag = 0;
                        }
                    }


                }
                else
                {

                    www();
                    MessageBox.Show("此卡无效", "Warning");
                   
                    cflag = 0;
                    ID.Visible = false;
                    textBox1.Visible = false;
                    button1.Visible = true;
                    textBox1.Text = "";
                }
                myReader.Close();
                conn.Close();
                
                myport.Close();

            }
            else {
                MessageBox.Show("请打开串口","Warning");
                ID.Visible = false;
                textBox1.Visible = false;
                button1.Visible = true;
            }
  
        }
        private void www()
        {
            int warning = 1;
            int j = 0;
            while (warning == 1)
            {

                byte[] send_warning = new byte[8];
                send_warning[0] = 0x11;
                send_warning[1] = 0x00;
                send_warning[2] = 0x00;
                send_warning[3] = 0x00;
                send_warning[4] = 0x00;
                send_warning[5] = 0x00;
                send_warning[6] = 0x33;
                send_warning[7] = 0x44;


                myport.Write(send_warning, 0, 8);
                  j++;
                if (j >= 1)
                {
                    warning = 0;
                }

            }

        }
        private void login_MouseEnter(object sender, EventArgs e)
        {
    
        }

        private void linkLabel1_MouseClick(object sender, MouseEventArgs e)
        {
         //  myport.Close();
            adminlogin form = new adminlogin();
            form.Show();
            this.Hide();
          //  myport.Close();
        }

        private void login_MouseLeave(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)  //初始化打开串口
        {
  
                    if (!myport.IsOpen)
            {
                
                myport.Open();
               
            }
            else {
                ;
            }
            ID.Visible = true;
            textBox1.Visible = true;
            button1.Visible = false;
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            pictureBox1.Visible = false;
            pictureBox2.Visible = true;
            pictureBox3.Visible = false;
            pictureBox4.Visible = false;
            linkLabel1.Visible = false;
            linkLabel2.Visible = false;
            linkLabel3.Visible = false;
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            pictureBox1.Visible = true;
            pictureBox2.Visible = false;
            pictureBox3.Visible = true;
            pictureBox4.Visible = false;
            linkLabel1.Visible = true;
            linkLabel2.Visible = true;
            linkLabel3.Visible = true;
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            pictureBox1.Visible = false;
            pictureBox2.Visible = false;
            pictureBox3.Visible = false;
            pictureBox4.Visible = true;
            linkLabel1.Visible = false;
            linkLabel2.Visible = false;
            linkLabel3.Visible = false;
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            pictureBox1.Visible = true;
            pictureBox2.Visible = false;
            pictureBox3.Visible = true;
            pictureBox4.Visible = false;
            linkLabel1.Visible = true;
            linkLabel2.Visible = true;
            linkLabel3.Visible = true;
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            pictureBox1.Visible = false;
            pictureBox2.Visible = true;
            pictureBox3.Visible = false;
            pictureBox4.Visible = false;
            linkLabel1.Visible = false;
            linkLabel2.Visible = false;
            linkLabel3.Visible = false;

        }

        private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            pictureBox1.Visible = false;
            pictureBox2.Visible = false;
            pictureBox3.Visible = false;
            pictureBox4.Visible = true;
            linkLabel1.Visible = false;
            linkLabel2.Visible = false;
            linkLabel3.Visible = false;
        }
    }
}
