const admin = require('firebase-admin');

// Load service account key
const serviceAccount = require('./serviceAccountKey.json');

// Initialize the Admin SDK
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: 'https://lifiaccesscontrol-default-rtdb.firebaseio.com/'  
});

// Get a Database reference
const db = admin.database();

// Start listening to all requests
db.ref('accessRequests').on('child_added', async requestSnap => {
  const req = requestSnap.val();
  const deviceId = req.deviceId;
  const capBits  = req.capBits;

  console.log(`→ New request from ${deviceId}: capBits=${capBits}`);

  // Lookup the device’s policy
  const devSnap = await db.ref(`userDevices/${deviceId}`).once('value');
  if (!devSnap.exists()) {
    console.error(`! Unknown device: ${deviceId}`);
    return;
  }
  const { userId, allowedCodes } = devSnap.val();

  // check and find allow or deny
  const allowed = !!allowedCodes[capBits];
  const status = allowed ? 'granted' : 'denied';

  // Write the log
  const logRef = db.ref('accessLogs').push();
  await logRef.set({
    requestId:   requestSnap.key,
    deviceId,
    userId,
    capBits,
    status,
    timestamp:   admin.database.ServerValue.TIMESTAMP
  });

  console.log(`✓ Logged ${status} for ${userId} on ${deviceId}`);
});

// Indicate service is running to check if it is working
console.log('AccessControl service running…');
