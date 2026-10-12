plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.libmailcore.androidexample"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.libmailcore.androidexample"
        // mailcore2-android-4.aar links its native deps at API 23, so minSdk must be >= 23.
        minSdk = 23
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        // The .aar ships these ABIs only.
        ndk { abiFilters += listOf("armeabi-v7a", "arm64-v8a", "x86") }
    }

    buildFeatures { viewBinding = true }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions { jvmTarget = "17" }

    buildTypes {
        release {
            isMinifyEnabled = false
        }
    }
}

dependencies {
    // mailcore2 as a local .aar (native libMailCore.so + com.libmailcore Java API)
    implementation(files("libs/mailcore2-android-4.aar"))

    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.appcompat:appcompat:1.6.1")
}
