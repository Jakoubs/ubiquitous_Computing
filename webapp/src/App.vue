<script setup lang="ts">
  import { ref } from 'vue';

  const apiUrl = ref('');
  async function fetchData() {
  }
  let ledcolor = ref("red");
  async function setColor(color: string) {
    ledcolor.value = color;
    fetch(apiUrl.value + "/?color=" + ledcolor.value)
  } 
</script>

<template>
  <div class="container">
    <h1>Lab 1</h1>       
    <input 
      v-model="apiUrl" 
      placeholder="API URL eingeben..." 
      class="url-input"
    />
     <div>
<div style="display: flex; align-items: center;">
<p>Current Color</p>
<p v-if="ledcolor=='red'" class="color">🔴</p>
<p v-if="ledcolor=='green'" class="color">🟢</p>
<p v-if="ledcolor=='blue'" class="color">🔵</p>
</div>
<div style="display: flex; gap: 10px;">
<button @click="setColor('red')" style="background-color: red;">Red</button>
<button @click="setColor('green')" style="background-color: green;">Green</button>
<button @click="setColor('blue')" style="background-color: blue;">Blue</button>
</div>
</div> 
  </div>
  <p>{{apiUrl }}</p>
</template>

<style scoped>
.url-input {
  width: 100%;
  padding: 8px;
  margin-bottom: 10px;
}
.result {
  background: #f4f4f4;
  padding: 10px;
  margin-top: 20px;
  border-radius: 4px;
}
</style>