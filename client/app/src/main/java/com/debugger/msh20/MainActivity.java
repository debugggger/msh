package com.debugger.msh20;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.IOException;
import java.util.StringTokenizer;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{

  ImageButton settingsButton;
  ImageButton syncButton;
  ImageView stateLamp1View;
  ImageView stateIllumin1View;
  Button lampButton;
  Button illuminateButton;
  TextView temperatureText;
  TextView humidityText;

  private SharedPreferences pref;
  private Handler mHandler = new Handler();

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN);
    setContentView(R.layout.activity_main);

    pref = getSharedPreferences("IP_Pref", MODE_PRIVATE);

    lampButton = (Button) findViewById(R.id.bLamp);
    temperatureText = (TextView) findViewById(R.id.tTemperature);
    humidityText = (TextView) findViewById(R.id.tHumidity);
    syncButton = (ImageButton) findViewById(R.id.bSync);
    stateLamp1View = (ImageView) findViewById(R.id.imLamp);
    stateIllumin1View = (ImageView) findViewById(R.id.imIllumination);
    illuminateButton = (Button) findViewById(R.id.bIllumination);

    settingsButton = (ImageButton) findViewById(R.id.bSettings);
    settingsButton.setOnClickListener(this);

    mHandler.removeCallbacks(synchr);
    mHandler.postDelayed(synchr, 1000);

    stateLamp1View.setColorFilter(Color.DKGRAY);
    stateIllumin1View.setColorFilter(Color.DKGRAY);

    lampButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("lamp1");
      }
    });
    syncButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("synclamp1");
      }
    });
    illuminateButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("illuminate1" + "@" + getColour());
      }
    });
  }

  @Override
  public void onClick(View v) {
    if (v.getId() == R.id.bSettings) {
      Intent intentSetting = new Intent(this, SettingsActivity.class);
      startActivity(intentSetting);
    }
  }

  private void post(String key){
    new Thread(new Runnable() {
      @Override
      public void run() {
        String url = "http://" + getIP() + "/" + key;
        Request request = new Request.Builder()
            .url(url)
            .build();

        new OkHttpClient().newCall(request)
            .enqueue(new Callback()
            {
              @Override
              public void onFailure(final Call call, IOException e)
              {
                e.printStackTrace();
              }
              @Override
              public void onResponse(Call call, final Response response) throws IOException
              {
                String res = response.body().string();
                switch (key){
                  case "lamp1":
                  case "synclamp1":
                    setImageState(res, stateLamp1View);
                    break;
                  case "illuminate1@White":
                  case "illuminate1@Red":
                  case "illuminate1@Blue":
                  case "illuminate1@Yellow":
                  case "illuminate1@Green":
                  case "illuminate1@Rainbow":
                  case "syncIllum1":
                    setImageState(res, stateIllumin1View);
                    break;
                  case "temperature":
                    runOnUiThread(new Runnable() {
                      @Override
                      public void run() {
                        temperatureText.setText(parseResponse(res) + " Cº");
                      }
                    });
                    break;
                  case "humidity":
                    runOnUiThread(new Runnable() {
                      @Override
                      public void run() {
                        humidityText.setText(parseResponse(res) + " %");
                      }
                    });
                    break;
                  default:
                    break;
                }
              }
            });

      }
    }).start();
  }

  private String parseResponse(String res){
    //String result = "";
    StringTokenizer st = new StringTokenizer(res, "@");
    String obj1 = st.nextToken();
    StringTokenizer obj1st = new StringTokenizer(obj1, "#");
    String obj10Stat = obj1st.nextToken();
    String result = obj1st.nextToken();

    return result;
  }

  private String getIP(){
    String ip = pref.getString("ip", "");
    if (ip != null){
      if(!ip.isEmpty()){
        return ip;
      }
    }
    return "192.168.0.123";
  }

  private String getColour(){
    String colour = pref.getString("colour", "");
    if (colour != null){
      if(!colour.isEmpty()){
        return colour;
      }
    }
    return "White";
  }

  private Runnable synchr = new Runnable() {
    @Override
    public void run()
    {
      post("temperature");
      post("humidity");
      post("synclamp1");
      post("syncIllum1");
      mHandler.postDelayed(this, 1000);
    }
  };

  private void setImageState(String res, ImageView param){
    if (parseResponse(res).equals("off")){
      param.setColorFilter(Color.RED);
    }
    else if (parseResponse(res).equals("on")){
      param.setColorFilter(Color.GREEN);
    }
  }
}