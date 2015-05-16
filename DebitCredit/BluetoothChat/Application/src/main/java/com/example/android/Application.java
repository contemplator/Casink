package com.example.android;

/**
 * Created by idlefox on 15/5/16.
 */

import com.parse.Parse;

public class Application extends android.app.Application {

    public void onCreate() {
        Parse.enableLocalDatastore(this);
        Parse.initialize(this, "GxQAcWbTF3xpE9zAgmjYHtdBAgY6Bg7nvkuFbZnO", "S8K2FpSPr2dccKefa4yTfGD0qwLHdldtsPOwH29G"); // Application ID, Client Key
    }

}
