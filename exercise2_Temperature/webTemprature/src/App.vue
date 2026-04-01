<script setup lang="ts">
import { RefSymbol } from '@vue/reactivity';
import { ElButton, ElCheckbox, ElInput } from 'element-plus';
import { ref, watch, onUnmounted,computed} from 'vue'
const temp = ref()
const borderMed = ref(20)
const borderHigh = ref(32)
const checked = ref(false)
const tempWidth = 500;
const MINCOLOR = 0;
const MAXCOLOR = 60;

function calcRed() {
  return (MAXCOLOR - borderHigh.value) / (MAXCOLOR - MINCOLOR) * 100;
}
function calcGreen() {
    return (borderHigh.value - borderMed.value) / (MAXCOLOR - MINCOLOR) * 100;
}
function calcBlue() {
      return (borderMed.value - MINCOLOR) / (MAXCOLOR - MINCOLOR) * 100;
}
async function updateMedBorder(val:number) { 
  fetch(url.value + '/?borderMed=' + val)
}
async function updateHighBorder(val:number) { 
  fetch(url.value + '/?borderHigh=' + val)
}
const url  = ref("http://10.42.0.18");

async function getTemp() {
  const targetUrl = `${url.value}/getTemp`;
  const response = await fetch(targetUrl, {
    method: 'GET',
    mode: 'cors' 
  });
  if (!response.ok) {
    throw new Error(`Server responded with status: ${response.status}`);
  }
  const jsonresp = await response.json();
  temp.value = jsonresp.temp; 
}
let timerId: ReturnType<typeof setInterval> | null = null;
watch(checked, (newValue) => {
  if (newValue) {
    // Start interval: 500ms = 0.5s
    getTemp(); // Run once immediately
    timerId = setInterval(getTemp, 500);
  } else {
    // Stop interval
    if (timerId) {
      clearInterval(timerId);
      timerId = null;
    }
  }
});
const bgColor = computed(() => {
  // Wichtig: .value nutzen!
  if (!temp.value && temp.value !== 0) return "gainsboro"; // Fallback, falls noch kein Wert da ist
  
  if (temp.value < borderMed.value) {
    return "lightblue";
  } else if (temp.value <= borderHigh.value) {
    return "lightgreen";
  } else {
    return "coral";
  }
});
</script>

<template>
  <h1>Lab1 Temprature</h1>
  <ElInput style="width: 300px;" v-model="url">
    <template #prefix>
      __url__
    </template>
  </ElInput>

  <el-input-number :min="MINCOLOR" :max="borderHigh" @change="updateMedBorder(borderMed)" v-model="borderMed" :step="1">
    <template #prefix>bg</template>
  </el-input-number>
  <el-input-number :min="borderMed" :max="MAXCOLOR" @change="updateHighBorder(borderHigh)" v-model="borderHigh" :step="1">
    <template #prefix>gr</template>
  </el-input-number>  
  <div class="content" :style="{'backgroundColor':bgColor}">
   <div style="margin: 15px;">
    <div class="colorRang" :style="{display: 'flex', width: tempWidth + 'px'}">
      <div class="blue" :style="{width: calcBlue() + '%'}"></div>
      <div class="green" :style="{width: calcGreen() + '%'}"></div>
      <div class="red" :style="{width: calcRed() + '%'}"></div>
    </div>
     <div class="footer" :style="{width: tempWidth +'px'}">
        <p class="temp" style="left: 0;">{{MINCOLOR}}C</p>
        <p class="temp" style="margin-left: auto;">{{MAXCOLOR}}C</p>
    </div>
  </div>
  </div>
  <div style="display: flex; gap: 10px; align-items: center;">
    <p>Temp:</p>
    <h2 style="font-size: 50px;">{{temp}}</h2>
  </div>
    <el-checkbox v-model="checked" label="Auto update" size="small" />
    </template>
<style scoped>
 .red {
    background-color: red;
    height: 20px;
  }
 .green {background-color: green;
    height: 20px;
}
 .blue {background-color: blue;
     height: 20px;
}
.footer{
  display: flex;
}
.temp{
  margin: 0;
}
.content{
  padding: 10px;
  margin: 10px;
  width: max-content;
  border-radius: 20px;
}
</style>