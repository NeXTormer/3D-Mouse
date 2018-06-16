using System.Collections;
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
	}
	



	void FixedUpdate ()
    {
        byte[] data = new byte[3];

        data[0] = Convert.ToByte(sliderRed.value);
        data[1] = Convert.ToByte(sliderGreen.value);
        data[2] = Convert.ToByte(sliderBlue.value);

        Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

        IPAddress serverAddr = IPAddress.Parse(receiver.mouseIP);

        IPEndPoint endPoint = new IPEndPoint(serverAddr, receiver.port);

        sock.SendTo(data, endPoint);
    }
}
