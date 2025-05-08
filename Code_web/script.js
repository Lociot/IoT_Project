// Sử lý chọn của thanh NavBar
document.addEventListener('DOMContentLoaded', function() {
    const links = document.querySelectorAll('.sidebar a');
    links.forEach(link => {
        link.addEventListener('click', function() {
            links.forEach(l => l.classList.remove('active'));
            this.classList.add('active');
        });
    });
});

// Nút nhấn ẩn thanh NavBar
function toggleNav() {
    const sidebar = document.getElementById('sidebar');
    const content = document.getElementById('main-content');
    sidebar.classList.toggle('collapsed');
    content.classList.toggle('collapsed');
}


// Thêm sự kiện khi tài liệu đã được tải hoàn toàn
document.addEventListener('DOMContentLoaded', function() {
    // Ẩn tất cả các phần ban đầu
    document.querySelectorAll('.navbar').forEach(section => {
        section.style.display = 'none';
    });

    // Hiển thị phần Home mặc định
    const homeSection = document.getElementById('Home');
    homeSection.style.display = 'block';
    homeSection.classList.add('show');
});

// Thêm sự kiện cho các liên kết điều hướng
document.querySelectorAll('.nav-link').forEach(link => {
    link.addEventListener('click', function(event) {
        event.preventDefault(); // Ngăn chặn hành vi mặc định của liên kết
        const targetId = this.getAttribute('data-target'); // Lấy ID của phần cần hiển thị
        // Ẩn tất cả các phần
        document.querySelectorAll('.navbar').forEach(section => {
            section.style.display = 'none';
            section.classList.remove('show');
        });
        // Hiển thị phần được chọn
        const targetSection = document.getElementById(targetId);
        targetSection.style.display = 'block';
        setTimeout(() => {
            targetSection.classList.add('show');
        }, 10); // Thêm một chút độ trễ để đảm bảo chuyển đổi hoạt động
    });
});


// device chỉnh từ firebase phòng HR 
 function listenToDeviceState(deviceId) {
    var dbRef13 = firebase.database().ref("HRroom/device" + deviceId);
    dbRef13.on("value", (snapshot) => {
        var state = snapshot.val().state;
        console.log("Trạng thái từ Firebase cho thiết bị", deviceId, ":", state);

        const switchElement = document.getElementById(`HR-SWITCH-${deviceId}`);
        const imgOn = document.getElementById(`HR-ON-${deviceId}`);
        const imgOff = document.getElementById(`HR-OFF-${deviceId}`);

        if (state === "on") {
            switchElement.checked = true;
            imgOn.style.display = "block";
            imgOff.style.display = "none";
        } else {
            switchElement.checked = false;
            imgOn.style.display = "none";
            imgOff.style.display = "block";
        }
    });
}
// device chỉnh từ web 
function updateDeviceState(deviceId, isOn) {
    var dbRef14 = firebase.database().ref("HRroom/device" + deviceId);
    dbRef14.set({
        state: isOn ? "on" : "off"
    }).then(() => {
        console.log(`Thiết bị ${deviceId} đã được cập nhật thành ${isOn ? "on" : "off"}`);
    }).catch((error) => {
        console.error("Lỗi khi cập nhật Firebase:", error);
    });
}

// Gán sự kiện cho các công tắc
document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("HR-SWITCH-1").addEventListener("change", function() {
        updateDeviceState(1, this.checked);
    });
    document.getElementById("HR-SWITCH-2").addEventListener("change", function() {
        updateDeviceState(2, this.checked);
    });
    document.getElementById("HR-SWITCH-3").addEventListener("change", function() {
        updateDeviceState(3, this.checked);
    });
    document.getElementById("HR-SWITCH-4").addEventListener("change", function() {
        updateDeviceState(4, this.checked);
    });
    // Gán cho các thiết bị khác...
    listenToDeviceState(1);
    listenToDeviceState(2);
    listenToDeviceState(3);
    listenToDeviceState(4);
    // Gọi hàm lắng nghe cho các thiết bị khác...
});

//-------------------------------------------------------------------------------------------------------
// device chỉnh từ firebase phòng IT
function listenToDeviceState1(deviceId) {
    var dbRef15 = firebase.database().ref("ITroom/device" + deviceId);
    dbRef15.on("value", (snapshot) => {
        var state = snapshot.val().state;
        console.log("Trạng thái từ Firebase cho thiết bị", deviceId, ":", state);

        const switchElement = document.getElementById(`IT-SWITCH-${deviceId}`);
        const imgOn = document.getElementById(`IT-ON-${deviceId}`);
        const imgOff = document.getElementById(`IT-OFF-${deviceId}`);

        if (state === "on") {
            switchElement.checked = true;
            imgOn.style.display = "block";
            imgOff.style.display = "none";
        } else {
            switchElement.checked = false;
            imgOn.style.display = "none";
            imgOff.style.display = "block";
        }
    });
}
// device chỉnh từ web 
function updateDeviceState1(deviceId, isOn) {
    var dbRef16 = firebase.database().ref("ITroom/device" + deviceId);
    dbRef16.set({
        state: isOn ? "on" : "off"
    }).then(() => {
        console.log(`Thiết bị ${deviceId} đã được cập nhật thành ${isOn ? "on" : "off"}`);
    }).catch((error) => {
        console.error("Lỗi khi cập nhật Firebase:", error);
    });
}

// Gán sự kiện cho các công tắc
document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("IT-SWITCH-1").addEventListener("change", function() {
        updateDeviceState1(1, this.checked);
    });
    document.getElementById("IT-SWITCH-2").addEventListener("change", function() {
        updateDeviceState1(2, this.checked);
    });
    document.getElementById("IT-SWITCH-3").addEventListener("change", function() {
        updateDeviceState1(3, this.checked);
    });
    document.getElementById("IT-SWITCH-4").addEventListener("change", function() {
        updateDeviceState1(4, this.checked);
    });
    // Gán cho các thiết bị khác...
    listenToDeviceState1(1);
    listenToDeviceState1(2);
    listenToDeviceState1(3);
    listenToDeviceState1(4);
    // Gọi hàm lắng nghe cho các thiết bị khác...
});

//------------------------------------------------------------------------------------------------------------
// device chỉnh từ firebase phòng Marketing
function listenToDeviceState2(deviceId) {
    var dbRef16 = firebase.database().ref("Marketingroom/device" + deviceId);
    dbRef16.on("value", (snapshot) => {
        var state = snapshot.val().state;
        console.log("Trạng thái từ Firebase cho thiết bị", deviceId, ":", state);

        const switchElement = document.getElementById(`Marketing-SWITCH-${deviceId}`);
        const imgOn = document.getElementById(`Marketing-ON-${deviceId}`);
        const imgOff = document.getElementById(`Marketing-OFF-${deviceId}`);

        if (state === "on") {
            switchElement.checked = true;
            imgOn.style.display = "block";
            imgOff.style.display = "none";
        } else {
            switchElement.checked = false;
            imgOn.style.display = "none";
            imgOff.style.display = "block";
        }
    });
}
// device chỉnh từ web 
function updateDeviceState2(deviceId, isOn) {
    var dbRef17 = firebase.database().ref("Marketingroom/device" + deviceId);
    dbRef17.set({
        state: isOn ? "on" : "off"
    }).then(() => {
        console.log(`Thiết bị ${deviceId} đã được cập nhật thành ${isOn ? "on" : "off"}`);
    }).catch((error) => {
        console.error("Lỗi khi cập nhật Firebase:", error);
    });
}

// Gán sự kiện cho các công tắc
document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("Marketing-SWITCH-1").addEventListener("change", function() {
        updateDeviceState2(1, this.checked);
    });
    document.getElementById("Marketing-SWITCH-2").addEventListener("change", function() {
        updateDeviceState2(2, this.checked);
    });
    document.getElementById("Marketing-SWITCH-3").addEventListener("change", function() {
        updateDeviceState2(3, this.checked);
    });
    document.getElementById("Marketing-SWITCH-4").addEventListener("change", function() {
        updateDeviceState2(4, this.checked);
    });
    // Gán cho các thiết bị khác...
    listenToDeviceState2(1);
    listenToDeviceState2(2);
    listenToDeviceState2(3);
    listenToDeviceState2(4);
    // Gọi hàm lắng nghe cho các thiết bị khác...
});

//-------------------------------------------------------------------------------------------------------------------
// Đảm bảo mã chỉ chạy sau khi DOM đã tải hoàn toàn
document.addEventListener('DOMContentLoaded', function() {
    const maxTime = 2 * 60 * 1000; // 2 phút tính bằng mili giây


    // Khởi tạo các biểu đồ
    const HRCharts = {
        temperature: initializeChart('HR-tempChart', 'Nhiệt độ (°C)', 'rgba(255, 99, 132, 1)', 'HR-tempChartContainer'),
        humidity: initializeChart('HR-humiChart', 'Độ ẩm (%)', 'rgba(54, 162, 235, 1)', 'HR-humidityChartContainer'),
        co2: initializeChart('HR-co2Chart', 'CO2 (ppm)', 'rgba(75, 192, 192, 1)', 'HR-co2ChartContainer'),
        dust: initializeChart('HR-dustChart', 'Bụi mịn (µg/m³)', 'rgba(153, 102, 255, 1)', 'HR-dustChartContainer') 
    };

    const ITCharts = {
        temperature: initializeChart('IT-tempChart', 'Nhiệt độ (°C)', 'rgba(255, 99, 132, 1)', 'IT-tempChartContainer'),
        humidity: initializeChart('IT-humiChart', 'Độ ẩm (%)', 'rgba(54, 162, 235, 1)', 'IT-humiChartContainer'),
        co2: initializeChart('IT-co2Chart', 'CO2 (ppm)', 'rgba(75, 192, 192, 1)', 'IT-co2ChartContainer'),
        dust: initializeChart('IT-dustChart', 'Bụi mịn (µg/m³)', 'rgba(153, 102, 255, 1)', 'IT-dustChartContainer')
    };    

    const MarketingCharts = {
        temperature: initializeChart('Marketing-tempChart', 'Nhiệt độ (°C)', 'rgba(255, 99, 132, 1)', 'Marketing-tempChartContainer'),
        humidity: initializeChart('Marketing-humiChart', 'Độ ẩm (%)', 'rgba(54, 162, 235, 1)', 'Marketing-humiChartContainer'),
        co2: initializeChart('Marketing-co2Chart', 'CO2 (ppm)', 'rgba(75, 192, 192, 1)', 'Marketing-co2ChartContainer'),
        dust: initializeChart('Marketing-dustChart', 'Bụi mịn (µg/m³)', 'rgba(153, 102, 255, 1)', 'Marketing-dustChartContainer')
    };    

    // Hàm khởi tạo biểu đồ
    function initializeChart(chartId, label, borderColor, containerId) {
        const ctx = document.getElementById(chartId)?.getContext('2d');
        return ctx ? new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: label,
                    data: [],
                    borderColor: borderColor,
                    borderWidth: 2,
                    fill: false
                }]
            },
            options: {
                responsive: true,
                scales: {
                    x: {
                        type: 'time',
                        time: {
                            unit: 'second',
                            tooltipFormat: 'HH:mm:ss',
                            displayFormats: { second: 'HH:mm:ss' }
                        },
                        title: { display: true, text: 'Thời gian' }
                    },
                    y: { title: { display: true, text: label } }
                }
            }
        }) : null;
    }

    // Hàm cập nhật dữ liệu cho các biểu đồ
    function updateCharts(tempValue, humidityValue, co2Value, dustValue, room) {
        const currentTime = new Date();
        let charts;
    
        // Chọn biểu đồ tương ứng với phòng
        if (room === 'HR') {
            charts = HRCharts;
        } else if (room === 'IT') {
            charts = ITCharts;
        } else if (room === 'Marketing') {
            charts = MarketingCharts;
        }
    
        const values = { temperature: tempValue, humidity: humidityValue, co2: co2Value, dust: dustValue };
    
        // Log dữ liệu cảm biến
        console.log(`Cập nhật dữ liệu cho ${room}:`, values);
    
        for (const [key, value] of Object.entries(values)) {
            if (value !== null && charts[key]) {
                charts[key].data.labels.push(currentTime);
                charts[key].data.datasets[0].data.push(value);
                limitChartData(charts[key]);
    
                // Log thông tin sau khi cập nhật biểu đồ
                console.log(`Cập nhật biểu đồ ${key} của ${room}:`, {
                    labels: charts[key].data.labels,
                    data: charts[key].data.datasets[0].data
                });
    
                charts[key].update();
            }
        }
    }    

    function limitChartData(chart) {
        const oldestTime = new Date(Date.now() - maxTime);
        while (chart.data.labels.length && chart.data.labels[0] < oldestTime) {
            chart.data.labels.shift();
            chart.data.datasets[0].data.shift();
        }
    }

    // Phần hiển thị biểu đồ khi click vào các thẻ
    function showChartHR(chartId) {
        document.querySelectorAll('.HR-chart-wrapper').forEach(chart => {
            chart.style.display = 'none';
        });
        document.getElementById(chartId).style.display = 'block';   
    }
    // Mặc định hiển thị biểu đồ nhiệt độ
    showChartHR('HR-tempChartContainer');
    document.getElementById('HR-tempCard').addEventListener('click', () => {
        showChartHR('HR-tempChartContainer');
    });

    document.getElementById('HR-humidityCard').addEventListener('click', () => {
        showChartHR('HR-humiChartContainer');
    });

    document.getElementById('HR-dustCard').addEventListener('click', () => {
        showChartHR('HR-dustChartContainer');
    });

    document.getElementById('HR-co2Card').addEventListener('click', () => {
        showChartHR('HR-co2ChartContainer');
    });

        // Phần sự kiện click cho ITroom
    function showChartIT(chartId) {
        document.querySelectorAll('.IT-chart-wrapper').forEach(chart => {
            chart.style.display = 'none';
        });
        document.getElementById(chartId).style.display = 'block';   
    }
    showChartIT('IT-tempChartContainer');
    document.getElementById('IT-tempCard').addEventListener('click', () => {
        showChartIT('IT-tempChartContainer');
    });

    document.getElementById('IT-humidityCard').addEventListener('click', () => {
        showChartIT('IT-humiChartContainer');
    });

    document.getElementById('IT-dustCard').addEventListener('click', () => {
        showChartIT('IT-dustChartContainer');
    });

    document.getElementById('IT-co2Card').addEventListener('click', () => {
        showChartIT('IT-co2ChartContainer');
    });

    // Phần sự kiện click cho Marketingroom
    function showChartMKT(chartId) {
        document.querySelectorAll('.Marketing-chart-wrapper').forEach(chart => {
            chart.style.display = 'none';
        });
        document.getElementById(chartId).style.display = 'block';   
    }
    showChartMKT('Marketing-tempChartContainer');
    document.getElementById('Marketing-tempCard').addEventListener('click', () => {
        showChartMKT('Marketing-tempChartContainer');
    });

    document.getElementById('Marketing-humidityCard').addEventListener('click', () => {
        showChartMKT('Marketing-humiChartContainer');
    });

    document.getElementById('Marketing-dustCard').addEventListener('click', () => {
        showChartMKT('Marketing-dustChartContainer');
    });

    document.getElementById('Marketing-co2Card').addEventListener('click', () => {
        showChartMKT('Marketing-co2ChartContainer');
    });


    dbRef1.on('value', (snapshot) => {
        const nhietdoValue = snapshot.val();
        updateCharts(nhietdoValue, null, null, null, 'HR'); 
    });
    
    dbRef2.on('value', (snapshot) => {
        const doamValue = snapshot.val();
        updateCharts(null, doamValue, null, null, 'HR'); 
    });
    
    dbRef3.on('value', (snapshot) => {
        const buiminValue = snapshot.val();
        updateCharts(null, null, buiminValue, null, 'HR'); 
    });
    
    dbRef4.on('value', (snapshot) => {
        const co2Value = snapshot.val();
        updateCharts(null, null, null, co2Value, 'HR'); 
    });    

    dbRef5.on('value', (snapshot) => {
        const nhietdoValue = snapshot.val();
        updateCharts(nhietdoValue, null, null, null, 'IT'); 
    });
    
    dbRef6.on('value', (snapshot) => {
        const doamValue = snapshot.val();
        updateCharts(null, doamValue, null, null, 'IT'); 
    });
    
    dbRef7.on('value', (snapshot) => {
        const buiminValue = snapshot.val();
        updateCharts(null, null, buiminValue, null, 'IT'); 
    });
    
    dbRef8.on('value', (snapshot) => {
        const co2Value = snapshot.val();
        updateCharts(null, null, null, co2Value, 'IT'); 
    });        

    dbRef9.on('value', (snapshot) => {
        const nhietdoValue = snapshot.val();
        updateCharts(nhietdoValue, null, null, null, 'Marketing'); 
    });
    
    dbRef10.on('value', (snapshot) => {
        const doamValue = snapshot.val();
        updateCharts(null, doamValue, null, null, 'Marketing'); 
    });
    
    dbRef11.on('value', (snapshot) => {
        const buiminValue = snapshot.val();
        updateCharts(null, null, buiminValue, null, 'Marketing'); 
    });
    
    dbRef12.on('value', (snapshot) => {
        const co2Value = snapshot.val();
        updateCharts(null, null, null, co2Value, 'Marketing'); 
    });          
});
