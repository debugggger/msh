package com.debugger.msh20;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{

  ImageButton settingsButton;
  ImageButton syncButton;
  Button lampButton;
  Button illuminateButton;
  TextView temperatureText;

  private SharedPreferences pref;
  private Handler mHandler = new Handler();

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    pref = getSharedPreferences("IP_Pref", MODE_PRIVATE);

    lampButton = (Button) findViewById(R.id.bLamp);
    illuminateButton = (Button) findViewById(R.id.bIllumination);
    temperatureText = (TextView) findViewById(R.id.tTemperature);
    syncButton = (ImageButton) findViewById(R.id.bSync);

    settingsButton = (ImageButton) findViewById(R.id.bSettings);
    settingsButton.setOnClickListener(this);

    mHandler.removeCallbacks(synchr);
    mHandler.postDelayed(synchr, 100);

    lampButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("lamp");
      }
    });
    illuminateButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("illuminate");
      }
    });
    /*syncButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        post("sync");
      }
    });*/
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
                  case "lamp":
                    lampButton.setText(res);
                  case "sync":
                    runOnUiThread(new Runnable() {
                      @Override
                      public void run() {
                        temperatureText.setText(res + "Cº");
                      }
                    });
                  default:
                    break;
                }
              }
            });

      }
    }).start();
  }

  private String getIP(){
    String ip = pref.getString("ip", "");
    if (ip != null){
      if(!ip.isEmpty()){
        return ip;
      }
    }
    return "192.168.0.100";
  }

  private Runnable synchr = new Runnable() {
    @Override
    public void run()
    {
      post("sync");
      mHandler.postDelayed(this, 10000);
    }
  };
}