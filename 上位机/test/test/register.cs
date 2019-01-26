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
    public partial class register : Form
    {
        public register()
        {
            InitializeComponent();
            textBox3.Text = "100";                  //初始化预存余额
            comboBox1.SelectedIndex = 00;           //初始化combox
            comboBox2.SelectedIndex = 00;
            comboBox3.SelectedIndex = 00;
            comboBox4.SelectedIndex = 00;
            myport = new SerialPort();              //初始化接口
            myport.PortName = "com3";
            myport.BaudRate = 9600;
            myport.StopBits = StopBits.One;
            myport.DataBits = 8;
            myport.Parity = Parity.None;
        }
        SerialPort myport = null;
        
        private void button1_Click(object sender, EventArgs e)  //注册
        {
            string card_id = "0000" + comboBox1.Text + comboBox2.Text + comboBox3.Text + comboBox4.Text;    //生成卡号
            decimal money = Convert.ToDecimal( textBox3.Text);

            if ((money >= 0) && (money < 1000)&&(textBox2.Text!=""))
            {
                //生成链接数据库字符串
                string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                //创建数据库连接
                OleDbConnection conn = new OleDbConnection(conStr);
                //打开数据库连接
                conn.Open();
                OleDbDataReader myReader = null;
                //sql语句        
                string sql = "select * from myTable where cstr(卡号)='" + card_id + "'";
                OleDbCommand cmd = new OleDbCommand(sql, conn);
                myReader = cmd.ExecuteReader();
                if (myReader.Read())
                {
                    MessageBox.Show("卡号已存在", "Warning");
                }
                else
                {

                    sql = "insert into myTable(卡号,权限位,户主姓名,挂失位,余额) values('" + card_id + "',0,'" + textBox2.Text + "',0,'" +money +"')";
                    //命令
                    OleDbCommand command = new OleDbCommand(sql, conn);
                    command.ExecuteNonQuery();
                    myReader.Close();
                    conn.Close();
                    conn.Open();

                    sql = "select * from myTable where cstr(卡号)='" + card_id + "'";
                    command = new OleDbCommand(sql, conn);
                    myReader = cmd.ExecuteReader();

                    if (myReader.Read())
                    {
                        MessageBox.Show("注册成功", "Warning");
                        button2.Visible = true;
                        
                    }
                    conn.Close();

                }
                conn.Close();
            }
            else {
                if (textBox2.Text == "")
                {
                    MessageBox.Show("户主姓名不能为空", "Warning");
                }
                else {
                    if (money < 0)
                    {
                        MessageBox.Show("预存余额不合法", "Warning");
                    }
                    else {
                        MessageBox.Show("卡内最多存放1000元", "Warning");
                    }
                }
              
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.Hide();
        }

        private void button2_Click(object sender, EventArgs e)  //打开接口
        {
            if (!myport.IsOpen)
            {

                myport.Open();

            }
            else
            {
                ;
            }
            button3.Visible = true;
        }

        private void button3_Click(object sender, EventArgs e)      //写卡
        {
            string mm = comboBox1.Text + comboBox2.Text + comboBox3.Text + comboBox4.Text;

            int aa = Convert.ToInt32(comboBox1.Text);
            int bb = Convert.ToInt32(comboBox2.Text);
            int cc = Convert.ToInt32(comboBox3.Text);
            int dd = Convert.ToInt32(comboBox4.Text);
           
                byte[] myCard = new byte[8];
                myCard[0] = 0x33;
                myCard[1] = sendByte(aa);
                myCard[2] = sendByte(bb);
                myCard[3] = sendByte(cc);
                myCard[4] = sendByte(dd);
                myCard[5] = 0xff;
                myCard[6] = 0x33;
                myCard[7] = 0x44;
                myport.Write(myCard, 0, 8);
            

            MessageBox.Show("写卡成功","Warning");
            myport.Close();
        }
        private byte sendByte(int n)
        {
            byte[] b = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12};
            byte x;
            x = b[n];

            return x;
        }
    }
}
