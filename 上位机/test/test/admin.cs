using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;      //接口
using System.Data.OleDb;    //数据库
namespace test
{
    public partial class myTab : Form
    {
        public myTab()
        {
            InitializeComponent();
            label1.Text = "您好，管理员" + adminlogin.tString + "！";  //从adminlogin携带用户名过来显示
            textBox2.Text = "0";    //初始化充值文本框
            textBox3.Text = "0";    //初始化扣费文本框
        }
        SerialPort myport = null;
        int flag = 0;
        int cflag = 0;
        int kflag = 0;
        public string id = "";
        decimal money;

        private void admin_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'myICDataSet.myTable' table. You can move, or remove it, as needed.
            this.myTableTableAdapter.Fill(this.myICDataSet.myTable);

        }

        private void admin_MouseEnter(object sender, EventArgs e)
        {
            
           
        }
        public static string byteToHexStr(byte[] bytes)     //HEX byte转string
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

        private void button1_Click(object sender, EventArgs e)
        {
            
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {

            string ss = textBox1.Text;
            id = textBox1.Text;

            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //建立数据库引擎连接
            OleDbConnection conn = new OleDbConnection(conStr);
            conn.Open();
            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where 卡号= '" + id + "'";
            OleDbCommand cmd = new OleDbCommand(sql, conn);

            myReader = cmd.ExecuteReader();
            if (!myReader.Read())
            {
                MessageBox.Show("没有查询到此用户！", "Warning");
            }
            else
            {
                money = myReader.GetDecimal(4);     //后面充值扣费用
                textBox4.Text = Convert.ToString(money);
                //建立适配器，通过SQL语句去搜索数据库
                OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + ss + "'", conn);
                //建立数据集
                DataSet datas = new DataSet();
                //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                mydata.Fill(datas, 0, 1, "myTable");
                //用显示控件来显示表
                tab.DataSource = datas.Tables[0];
                conn.Close();
            }
        }

        private void textBox1_MouseClick(object sender, MouseEventArgs e)
        {
            
        }

        private void admin_KeyPress(object sender, KeyPressEventArgs e)
        {
           
        }

        private void button2_Click(object sender, EventArgs e)  //初始化接口
        {
            myport = new SerialPort();      
            myport.PortName = "com3";
            myport.BaudRate = 9600;
            myport.StopBits = StopBits.One;
            myport.DataBits = 8;
            myport.Parity = Parity.None;
            myport.Open();
            flag = 1;
            cflag = 0;
        }

        private void button3_Click(object sender, EventArgs e)  //读卡号
        {
            string ss = "";
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
            id = textBox1.Text;

            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //建立数据库引擎连接
            OleDbConnection conn = new OleDbConnection(conStr);
            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where cstr(卡号)='" + ss + "'";
            //命令
            conn.Open();
            OleDbCommand cmd = new OleDbCommand(sql, conn);
        
            myReader = cmd.ExecuteReader();
        
            if (myReader.Read())
            {
                money = myReader.GetDecimal(4);
                textBox4.Text = Convert.ToString(money);
                //建立适配器，通过SQL语句去搜索数据库
                OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + ss + "'", conn);
                //建立数据集
                DataSet datas = new DataSet();
                //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                mydata.Fill(datas, 0, 1, "myTable");
                //用显示控件来显示表
                tab.DataSource = datas.Tables[0];
                conn.Close();
                myport.Close();
            }
            else {
                MessageBox.Show("未查询到此用户","Warning");
                myport.Close();
            }
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            if (flag == 1)
            {
                myport.Close();
            }

            this.Hide();
            login form = new login();
            form.Show();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            register form = new register();
            form.Show();
        }

        private void button4_Click(object sender, EventArgs e)  //充值
        {

            decimal money = Convert.ToDecimal(textBox2.Text);
            if (money <= 0)
            {
                MessageBox.Show("数值不合法", "Warning");
            }
            else
            {

                //生成链接数据库字符串
                string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                //创建数据库连接
                OleDbConnection conn = new OleDbConnection(conStr);
                //打开数据库连接
                conn.Open();

                OleDbDataReader myReader = null;
                //sql语句        
                string sql = "select * from myTable where 卡号= '" + id + "'";
                OleDbCommand cmd = new OleDbCommand(sql, conn);

                myReader = cmd.ExecuteReader();
                if (myReader.Read() && (!myReader.GetBoolean(3))) //查询到且不挂失
                {

                    decimal mm = myReader.GetDecimal(4);    //读余额
                    money = money + mm;
                    if (money > 999)
                    {
                        MessageBox.Show("卡内最存放1000元,请重新充值", "Warning");
                    }
                    else
                    {

                        sql = "update myTable set 余额='" + money + "' where 卡号='" + textBox1.Text + "'";
                        cmd = new OleDbCommand(sql, conn);
                        if (cmd.ExecuteNonQuery() > 0)
                        {
                            MessageBox.Show("充值成功", "Warning");
                        }
                        else
                        {
                            MessageBox.Show("充值失败", "Warning");

                        }
                    }

                }
                else
                {
                    MessageBox.Show("此卡已挂失", "Warning");
                }
                OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
                //建立数据集
                DataSet datas = new DataSet();
                //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                mydata.Fill(datas, 0, 1, "myTable");
                //用显示控件来显示表
                tab.DataSource = datas.Tables[0];
                conn.Close();
            }
        }

        private void button1_Click_1(object sender, EventArgs e)    //挂失操作
        {
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();

            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where 卡号= '" + id + "'";
            OleDbCommand cmd = new OleDbCommand(sql, conn);
            myReader = cmd.ExecuteReader();
            if (myReader.Read() && (!myReader.GetBoolean(3)))   //查询到且没有挂失
            {
                sql = "update myTable set 挂失位='1' where 卡号='" + textBox1.Text + "'";
                cmd = new OleDbCommand(sql, conn);
                if (cmd.ExecuteNonQuery() > 0)
                {
                    MessageBox.Show("操作成功", "Warning");

                }
                else
                {
                    MessageBox.Show("操作失败", "Warning");
                }
            }
            else
            {
                MessageBox.Show("此卡已挂失", "Warning");
            }
            OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
            //建立数据集
            DataSet datas = new DataSet();
            //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
            mydata.Fill(datas, 0, 1, "myTable");
            //用显示控件来显示表
            tab.DataSource = datas.Tables[0];
            conn.Close();

        }

        private void button6_Click(object sender, EventArgs e)  //取消挂失
        {
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();

            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where 卡号= '" + id + "'";
            OleDbCommand cmd = new OleDbCommand(sql, conn);
            myReader = cmd.ExecuteReader();
            if (myReader.Read() && (myReader.GetBoolean(3)))   //查询到且挂失了
            {
                sql = "update myTable set 挂失位='0' where 卡号='" + textBox1.Text + "'";
                cmd = new OleDbCommand(sql, conn);
                if (cmd.ExecuteNonQuery() > 0)
                {
                    MessageBox.Show("操作成功", "Warning");

                }
                else
                {
                    MessageBox.Show("操作失败", "Warning");
                }
            }
            else
            {
                MessageBox.Show("此卡未挂失", "Warning");
            }
            OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
            //建立数据集
            DataSet datas = new DataSet();
            //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
            mydata.Fill(datas, 0, 1, "myTable");
            //用显示控件来显示表
            tab.DataSource = datas.Tables[0];
            conn.Close();
        }

        private void button5_Click(object sender, EventArgs e)  //注销用户
        {
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();

          //  OleDbDataReader myReader = null;
            //sql语句        
            string sql = "delete from myTable where 卡号= '" + id + "'";
            OleDbCommand cmd = new OleDbCommand(sql, conn);

            if (cmd.ExecuteNonQuery() > 0)
            {
                MessageBox.Show("用户注销成功", "Warning");
            }
            else {
                MessageBox.Show("用户注销失败", "Warning");
            }
            OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
            //建立数据集
            DataSet datas = new DataSet();
            //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
            mydata.Fill(datas, 0, 1, "myTable");
            //用显示控件来显示表
            tab.DataSource = datas.Tables[0];
            conn.Close();
        }

        private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
           
        }

        private void linkLabel4_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            
        }

        private void button7_Click(object sender, EventArgs e)  //扣费
        {
            decimal money = Convert.ToDecimal(textBox3.Text);
            if (money <= 0)
            {
                MessageBox.Show("数值不合法", "Warning");
            }
            else
            {

                //生成链接数据库字符串
                string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                //创建数据库连接
                OleDbConnection conn = new OleDbConnection(conStr);
                //打开数据库连接
                conn.Open();

                OleDbDataReader myReader = null;
                //sql语句        
                string sql = "select * from myTable where 卡号= '" + id + "'";
                OleDbCommand cmd = new OleDbCommand(sql, conn);

                myReader = cmd.ExecuteReader();
                if (myReader.Read() && (!myReader.GetBoolean(3))) //查询到且不挂失
                {

                    decimal mm = myReader.GetDecimal(4);    //读余额
                    money = mm-money;
                    if (money <0)
                    {
                        MessageBox.Show("卡内余额不足", "Warning");
                    }
                    else
                    {

                        sql = "update myTable set 余额='" + money + "' where 卡号='" + textBox1.Text + "'";
                        cmd = new OleDbCommand(sql, conn);
                        if (cmd.ExecuteNonQuery() > 0)
                        {
                            MessageBox.Show("扣费成功", "Warning");
                        }
                        else
                        {
                            MessageBox.Show("扣费失败", "Warning");

                        }
                    }

                }
                else
                {
                    MessageBox.Show("此卡已挂失", "Warning");
                }

                OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
                //建立数据集
                DataSet datas = new DataSet();
                //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                mydata.Fill(datas, 0, 1, "myTable");
                //用显示控件来显示表
                tab.DataSource = datas.Tables[0];
                conn.Close();
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //建立数据库引擎连接
            OleDbConnection conn = new OleDbConnection(conStr);
            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where cstr(卡号)='" + textBox1.Text + "'";
            //命令
            conn.Open();
            OleDbCommand cmd = new OleDbCommand(sql, conn);
            myReader = cmd.ExecuteReader();
            decimal mm = 0;
            if (myReader.Read())
            {
                mm = myReader.GetDecimal(4);
            }

            kflag = 0;
            button8.Visible = false;
            button9.Visible = true;
            while (kflag == 0)
            {
                    mm = mm - Convert.ToDecimal(0.6);
                if (mm <=Convert.ToDecimal( 0))
                {
                    MessageBox.Show("余额不足", "Warning");
                    
                    kflag = 1;
                    
                    conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                    //创建数据库连接
                    conn = new OleDbConnection(conStr);
                    //打开数据库连接
                    conn.Open();
                    sql = "update myTable set 余额='" + 0 + "' where 卡号='" + textBox1.Text + "'";
                    cmd = new OleDbCommand(sql, conn);
                    cmd.ExecuteNonQuery();


                    OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
                    //建立数据集
                    DataSet datas = new DataSet();
                    //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                    mydata.Fill(datas, 0, 1, "myTable");
                    //用显示控件来显示表
                    tab.DataSource = datas.Tables[0];
                    conn.Close();

                    button8.Visible = true;
                    button9.Visible = false;


                }
                else
                {
                    textBox4.Text = Convert.ToString(mm);
                    Application.DoEvents();
                    System.Threading.Thread.Sleep(100);
                }

            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            
            kflag = 1;
            decimal mm = Convert.ToDecimal(textBox4.Text);
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();
            string sql = "update myTable set 余额='" + mm + "' where 卡号='" + textBox1.Text + "'";
            OleDbCommand cmd = new OleDbCommand(sql, conn);
            cmd.ExecuteNonQuery();


            OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + id + "'", conn);
            //建立数据集
            DataSet datas = new DataSet();
            //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
            mydata.Fill(datas, 0, 1, "myTable");
            //用显示控件来显示表
            tab.DataSource = datas.Tables[0];
            conn.Close();

            button8.Visible = true;
            button9.Visible = false;


        }
    }
}
