import { initializeApp } from "https://www.gstatic.com/firebasejs/10.0.0/firebase-app.js";
import { getAuth, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, onAuthStateChanged } from "https://www.gstatic.com/firebasejs/10.0.0/firebase-auth.js";
import { getFirestore, collection, getDocs } from "https://www.gstatic.com/firebasejs/10.0.0/firebase-firestore.js";

const firebaseConfig = {
    apiKey: "AIzaSyBzKkjdFc8iIL7acumDhwZ20qNLwjAl4U8",
    authDomain: "iot-project-d423a.firebaseapp.com",
    projectId: "iot-project-d423a",
    storageBucket: "iot-project-d423a.appspot.com",
    messagingSenderId: "264900316324",
    appId: "1:264900316324:web:c5568ffa9904d537a1aa31",
    measurementId: "G-PC8Q4LBQFH"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const db = getFirestore(app);
let checkSlotsInterval;
let userRequestedNotification = false;

function showLogin() {
    document.getElementById("homeContainer").style.display = "none";
    document.getElementById("signupContainer").style.display = "none";
    document.getElementById("loginContainer").style.display = "block";
}

function showSignup() {
    document.getElementById("homeContainer").style.display = "none";
    document.getElementById("loginContainer").style.display = "none";
    document.getElementById("signupContainer").style.display = "block";
}

function showDashboard() {
    document.getElementById("homeContainer").style.display = "none";
    document.getElementById("signupContainer").style.display = "none";
    document.getElementById("loginContainer").style.display = "none";
    document.getElementById("dashboardContainer").style.display = "block";
    document.getElementById("containerSlots").style.display = "block";
    fetchSlots();
}

async function signup() {
    const email = document.getElementById("signupEmail").value;
    const password = document.getElementById("signupPassword").value;

    try {
        await createUserWithEmailAndPassword(auth, email, password);
        showDashboard();
    } catch (error) {
        console.error(error.message);
    }
}

async function login() {
    const email = document.getElementById("loginEmail").value;
    const password = document.getElementById("loginPassword").value;

    try {
        await signInWithEmailAndPassword(auth, email, password);
        showDashboard();
    } catch (error) {
        console.error("Invalid email or password");
    }
}

async function logout() {
    await signOut(auth);
    document.getElementById("dashboardContainer").style.display = "none";
    document.getElementById("containerSlots").style.display = "none";

    showHome();
}

function showHome() {
    document.getElementById("homeContainer").style.display = "block";
    document.getElementById("signupContainer").style.display = "none";
    document.getElementById("loginContainer").style.display = "none";
    document.getElementById("dashboardContainer").style.display = "none";
    document.getElementById("containerSlots").style.display = "none";
}

async function fetchSlots() {
    const slotsRef = collection(db, "slots");
    const slotsSnapshot = await getDocs(slotsRef);
    const slots = document.querySelectorAll(".slot"); // Get all slot divs
    let availableSlot = null;

    let availableCount = 0;
    let slotElements = Array.from(slots); // Convert NodeList to Array

    let index = 0;

    slotsSnapshot.forEach((doc) => {
        const slotData = doc.data();

        slotElements[index].style.backgroundColor = (slotData.status === 1 ? "green" : "red");
        if (slotData.status === 1 && !availableSlot) {
            availableSlot = slotData.id;
        }
        if (slotData.status === 1) availableCount++;
        index++;
    });

    document.querySelector("#slotCount").innerText = `Number of slots available: ${availableCount}`;

    window.availableSlot = availableSlot;

    if (userRequestedNotification) {
        updateNotifyMessage(); // Only update if user clicked Notify
    }
}

function notifyUser() {
    // const notifyMessage = document.getElementById("notifyMessage");
    // notifyMessage.id = "notify-active";
    userRequestedNotification = true; // Mark that user clicked Notify
    startCheckingSlots(); // Start fetching every 2 seconds
    updateNotifyMessage(); // Show initial message

}

function updateNotifyMessage() {
    const messageContainer = document.getElementById("notifyMessage");

    if (window.availableSlot) {
        messageContainer.innerText = `Wait is over! 🎉 A parking slot 🚗 is now available.\nGrab your spot at Slot: ${window.availableSlot}! 😊`;
        messageContainer.style.color = "green";
    } else {
        messageContainer.innerText = "All parking slots 🚗 are currently occupied!\n Don't worry, we'll notify you as soon as one becomes available. Stay tuned! 🔔";
        messageContainer.style.color = "red";
    }
}

function startCheckingSlots() {
    if (!checkSlotsInterval) {
        checkSlotsInterval = setInterval(fetchSlots, 1000);
    }
}

onAuthStateChanged(auth, (user) => {
    console.log(user);

    if (user) {
        showDashboard();
    } else {
        showHome();
    }
});

window.showLogin = showLogin;
window.showSignup = showSignup;
window.signup = signup;
window.login = login;
window.logout = logout;
window.showHome = showHome;
window.notifyUser = notifyUser