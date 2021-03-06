﻿using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;
using System;

public class RGBColor : MonoBehaviour {

    public Slider sliderRed;
    public Slider sliderGreen;
    public Slider sliderBlue;

    private NetworkReceiver receiver;


	void Start () {
        receiver = GetComponent<NetworkReceiver>();
        StartCoroutine("SendColor");
	}

    IEnumerator SendColor()
    {
        for (;;)
        { 
            if (receiver.receivedIP)
            {
                byte[] data = new byte[3];

                data[0] = Convert.ToByte(sliderRed.value);
                data[1] = Convert.ToByte(sliderGreen.value);
                data[2] = Convert.ToByte(sliderBlue.value);

                Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                IPEndPoint endPoint = new IPEndPoint(receiver.mouseIP, NetworkReceiver.PORT);

                sock.SendTo(data, endPoint);

            }
            yield return new WaitForSeconds(.005f);
        }
    }
}
