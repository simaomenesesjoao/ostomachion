#!/usr/bin/env node
import fs from 'fs';
import path from 'path';
import { marked } from 'marked';

// Usage: node install.js <destination_root>
// Example: node install.js ./public/html

if (process.argv.length < 3) {
  console.error('Usage: node install.js <destination_root>');
  process.exit(1);
}

const sourceRoot = process.cwd(); // current working directory (where you run the script)
const destRoot = path.resolve(process.argv[2]);

const katexCSS = `<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.css">`;
const katexJS = `
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.js"></script>
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/contrib/auto-render.min.js"></script>
<script>
  document.addEventListener("DOMContentLoaded", () => {
    renderMathInElement(document.body, {
      delimiters: [
        {left: "$$", right: "$$", display: true},
        {left: "$", right: "$", display: false}
      ]
    });
  });
</script>
`;

function convertMdToHtml(mdContent, title = '') {
  const htmlBody = marked.parse(mdContent);
  return `<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>${title}</title>
${katexCSS}
<style>
  body { max-width: 800px; margin: 2rem auto; font-family: sans-serif; padding: 0 1rem; background-color: white; color: black; }
  img { max-width: 100%; height: auto; }
  pre { background: #f6f8fa; padding: 1rem; overflow-x: auto; }
  code { font-family: monospace; }
  @media (prefers-color-scheme: dark) {
    body { background-color: #121212; color: #e0e0e0; }
    pre { background-color: #1e1e1e; }
  }
</style>
${katexJS}
</head>
<body>
${htmlBody}
</body>
</html>`;
}

function ensureDirExists(dirPath) {
  if (!fs.existsSync(dirPath)) {
    fs.mkdirSync(dirPath, { recursive: true });
  }
}

function processFolder(currentSrcFolder, currentDestFolder) {
  ensureDirExists(currentDestFolder);

  const items = fs.readdirSync(currentSrcFolder, { withFileTypes: true });

  for (const item of items) {
    const srcPath = path.join(currentSrcFolder, item.name);
    const destPath = path.join(currentDestFolder, item.name);

    if (item.isDirectory()) {
      processFolder(srcPath, destPath);
    } else if (item.isFile() && item.name.endsWith('.md')) {
      let mdContent = fs.readFileSync(srcPath, 'utf-8');

      // Optional: fix image paths here if needed (same folder-relative adjustment)
      const basePath = './';

      mdContent = mdContent.replace(/(!\[.*?\]\()(.+?)(\))/g, (_, p1, p2, p3) => {
        if (p2.startsWith('http') || p2.startsWith('/') || p2.startsWith(basePath)) return p1 + p2 + p3;
        return p1 + basePath + p2 + p3;
      });

      mdContent = mdContent.replace(/<img\s+([^>]*?)src=["'](?!https?:|\/|data:)([^"']+)["'](.*?)>/gi,
        (_, before, src, after) => `<img ${before}src="${basePath + src}"${after}>`
      );

      const htmlContent = convertMdToHtml(mdContent, item.name.replace('.md', ''));

      const destHtmlPath = destPath.replace(/\.md$/, '.html');
      fs.writeFileSync(destHtmlPath, htmlContent);

      console.log(`Converted: ${srcPath} → ${destHtmlPath}`);
    }
  }
}

processFolder(sourceRoot, destRoot);
