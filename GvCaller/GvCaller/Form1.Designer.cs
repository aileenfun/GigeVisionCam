namespace GvCaller
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button2 = new System.Windows.Forms.Button();
            this.tb_expo = new System.Windows.Forms.TextBox();
            this.tb_xend = new System.Windows.Forms.TextBox();
            this.tb_xstart = new System.Windows.Forms.TextBox();
            this.tb_ystart = new System.Windows.Forms.TextBox();
            this.tb_yend = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.cb_roienable = new System.Windows.Forms.CheckBox();
            this.cb_expoauto = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 40);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(711, 619);
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(839, 335);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "start";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // tb_expo
            // 
            this.tb_expo.Location = new System.Drawing.Point(814, 424);
            this.tb_expo.Name = "tb_expo";
            this.tb_expo.Size = new System.Drawing.Size(100, 21);
            this.tb_expo.TabIndex = 3;
            // 
            // tb_xend
            // 
            this.tb_xend.Location = new System.Drawing.Point(814, 173);
            this.tb_xend.Name = "tb_xend";
            this.tb_xend.Size = new System.Drawing.Size(100, 21);
            this.tb_xend.TabIndex = 4;
            // 
            // tb_xstart
            // 
            this.tb_xstart.Location = new System.Drawing.Point(814, 146);
            this.tb_xstart.Name = "tb_xstart";
            this.tb_xstart.Size = new System.Drawing.Size(100, 21);
            this.tb_xstart.TabIndex = 5;
            // 
            // tb_ystart
            // 
            this.tb_ystart.Location = new System.Drawing.Point(814, 200);
            this.tb_ystart.Name = "tb_ystart";
            this.tb_ystart.Size = new System.Drawing.Size(100, 21);
            this.tb_ystart.TabIndex = 6;
            // 
            // tb_yend
            // 
            this.tb_yend.Location = new System.Drawing.Point(814, 227);
            this.tb_yend.Name = "tb_yend";
            this.tb_yend.Size = new System.Drawing.Size(100, 21);
            this.tb_yend.TabIndex = 7;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(767, 427);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "Expo";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(767, 149);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 9;
            this.label2.Text = "xstart";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(767, 173);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "xend";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(767, 203);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 11;
            this.label4.Text = "ystart";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(767, 236);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 12;
            this.label5.Text = "yend";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(780, 368);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(131, 12);
            this.label6.TabIndex = 13;
            this.label6.Text = "(ROI在开启后不可设置)";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(839, 254);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 14;
            this.button1.Text = "setROI";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(839, 452);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 15;
            this.button3.Text = "setExpo";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // cb_roienable
            // 
            this.cb_roienable.AutoSize = true;
            this.cb_roienable.Location = new System.Drawing.Point(769, 261);
            this.cb_roienable.Name = "cb_roienable";
            this.cb_roienable.Size = new System.Drawing.Size(60, 16);
            this.cb_roienable.TabIndex = 16;
            this.cb_roienable.Text = "Enable";
            this.cb_roienable.UseVisualStyleBackColor = true;
            // 
            // cb_expoauto
            // 
            this.cb_expoauto.AutoSize = true;
            this.cb_expoauto.Location = new System.Drawing.Point(769, 459);
            this.cb_expoauto.Name = "cb_expoauto";
            this.cb_expoauto.Size = new System.Drawing.Size(48, 16);
            this.cb_expoauto.TabIndex = 17;
            this.cb_expoauto.Text = "auto";
            this.cb_expoauto.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(780, 346);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 18;
            this.label7.Text = "开始采集";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(980, 692);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.cb_expoauto);
            this.Controls.Add(this.cb_roienable);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tb_yend);
            this.Controls.Add(this.tb_ystart);
            this.Controls.Add(this.tb_xstart);
            this.Controls.Add(this.tb_xend);
            this.Controls.Add(this.tb_expo);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox tb_expo;
        private System.Windows.Forms.TextBox tb_xend;
        private System.Windows.Forms.TextBox tb_xstart;
        private System.Windows.Forms.TextBox tb_ystart;
        private System.Windows.Forms.TextBox tb_yend;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.CheckBox cb_roienable;
        private System.Windows.Forms.CheckBox cb_expoauto;
        private System.Windows.Forms.Label label7;
    }
}

