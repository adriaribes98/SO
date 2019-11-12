using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace Game
{
    public partial class Form1 : Form
    {
        Socket servidor;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                string mensaje = "1/" + textBox1.Text;
                //enviar al servidor
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);
                //respuesta del servidor
                byte[] msg2 = new byte[512];
                servidor.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
            }

            if (checkBox2.Checked)
            {
                string mensaje = "2/" + textBox1.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);

                byte[] msg2 = new byte[512];
                servidor.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
            }   

            if (checkBox3.Checked)
            {
                string mensaje = "3/" + textBox1.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);

                byte[] msg2 = new byte[512];
                servidor.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
            }   
                

            
            
        }

        

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            


        }

      

        private void button3_Click(object sender, EventArgs e)
        {
            string mensaje = "0/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            servidor.Send(msg);

            byte[] msg2 = new byte[512];
            servidor.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show(mensaje);
            
            //Desconectamos
            servidor.Shutdown(SocketShutdown.Both);
            servidor.Close();

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            //datos del servidor al que queremos conectarnos
            IPAddress direccion = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direccion, 9070);


            servidor = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                //Crear el socket
                servidor.Connect(ipep);//conectar el socket
                this.BackColor = Color.LightGreen;
            }


            catch (SocketException)
            {
                //si hay error sale el mensaje de error y salimos del programa
                MessageBox.Show("No se pudo establecer conexión");
            }

        }
    }
}
        

   


        
